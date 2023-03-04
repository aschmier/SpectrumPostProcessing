#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TH2D.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"
#include "/home/austin/alice/SubstructureAnalysis/helpers/math.C"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"

void filterResponse(TString fpath, TString fout)
{
    int minradius = 2;
    int maxradius = 6;
    Double_t textSize = 0.03;
    TString fileType = "png";
    double xmax = 240.;
    double ymax = 600.;

    TH2 *arrResponse[5];
    TH2 *arrResponseRebinned[5];
    TH2 *arrResponseClosure[5];
    TH2 *arrResponseClosureRebinned[5];
    TH2 *arrResponseNoClosure[5];
    TH2 *arrResponseNoClosureRebinned[5];
    double arrMCScale[5];

    auto binningpart = getJetPtBinningNonLinTrue8TeV(),
         binningdet  = getJetPtBinningNonLinSmear8TeV();

    gSystem->Exec("mkdir -p "+fout);
    gSystem->Exec("mkdir -p "+fout+"/Standard");
    gSystem->Exec("mkdir -p "+fout+"/Closure");
    gSystem->Exec("mkdir -p "+fout+"/TruthClosure");


    for(int bin = 1; bin <= 20; bin++){
        TFile *f = TFile::Open(Form("%s/%02i/AnalysisResults.root",fpath.Data(),bin), "read");
        if(!f || f->IsZombie()){
            cout << "File for pT-hard bin " << bin << " not found!" << endl;
            return;
        }

        for(int radius = minradius; radius <= maxradius; radius++){
            TString dirname = Form("EnergyScaleResults_FullJet_R0%i_INT7_default", radius);
            TString listname = Form("EnergyScaleHists_FullJet_R0%i_INT7_default", radius);

            TDirectory *dir = (TDirectory*)f->Get(dirname.Data());
            TList *list = (TList*)dir->Get(listname.Data());
            TH2 *response = (TH2*)list->FindObject("hJetResponseFine");
            TH2 *responseClosure = (TH2*)list->FindObject("hJetResponseFineClosure");
            TH2 *responseNoClosure = (TH2*)list->FindObject("hJetResponseFineNoClosure");
            TH1D *histtrials = (TH1D*)list->FindObject("fHistTrials");
            TH1D *histxsec = (TH1D*)list->FindObject("fHistXsection");
            TH2 *responsefine = (TH2*)response->Clone(Form("responsefine_R0%i_bin%i", radius, bin));

            double trials;
            for(int binTrials = 1; binTrials <= histtrials->GetNbinsX(); binTrials++){
                if(binTrials == 1) trials = histtrials->GetBinContent(binTrials);
                else if(histtrials->GetBinContent(binTrials) > trials) trials = histtrials->GetBinContent(binTrials);
            }

            double xsec;
            for(int binXS = 1; binXS <= histxsec->GetNbinsX(); binXS++){
                if(binXS == 1) xsec = histxsec->GetBinContent(binXS);
                else if(histxsec->GetBinContent(binXS) > xsec) xsec = histxsec->GetBinContent(binXS);
            }

            double mcscale = xsec / trials;
            arrMCScale[radius-minradius] = mcscale;

            response->SetNameTitle(Form("Rawresponse_R0%i", radius), Form("Raw response R=0.%i", radius));
            response->SetName(Form("%s_fine", response->GetName()));
            auto rebinnedresponse  = makeRebinned2D(response, binningdet, binningpart);
            rebinnedresponse->SetName(Form("%s_standard", rebinnedresponse->GetName()));
            response->SetName(Form("fineresponse_R0%i",radius));

            for(int x = 1; x <= rebinnedresponse->GetNbinsX(); x++){
                for(int y = 1; y <= rebinnedresponse->GetNbinsY(); y++){
                    float lowXval = rebinnedresponse->GetXaxis()->GetBinLowEdge(x);
                    float lowYval = rebinnedresponse->GetYaxis()->GetBinLowEdge(y);
                    float upXval  = rebinnedresponse->GetXaxis()->GetBinUpEdge(x);
                    float upYval  = rebinnedresponse->GetYaxis()->GetBinUpEdge(y);

                    int lowXbin = response->GetXaxis()->FindBin(lowXval);
                    int lowYbin = response->GetYaxis()->FindBin(lowYval);
                    int upXbin = response->GetXaxis()->FindBin(upXval);
                    int upYbin = response->GetYaxis()->FindBin(upYval);

                    double content = rebinnedresponse->GetBinContent(x,y);
                    if(content < 10){
                        //if(content > 0 && lowYval > 20 && upYval < 240){
                        //    cout << "Radius = 0." << radius << "; pT-Hard Bin: " << bin << endl;
                        //    cout << "pT Det: " << lowXval << " - " << upXval << endl;
                        //    cout << "pT Part: " << lowYval << " - " << upYval << endl;
                        //    cout << "Content: " << content << endl << endl;
                        //}
                        rebinnedresponse->SetBinContent(x,y,0.);
                        for(int finebinX = lowXbin; finebinX <= upXbin; finebinX++){
                            for(int finebinY = lowYbin; finebinY <= upYbin; finebinY++){
                                response->SetBinContent(finebinX,finebinY,0);
                            }
                        }
                    }
                }
            }

            for(int maxX = response->GetXaxis()->FindBin(xmax+1.); maxX <= response->GetNbinsX(); maxX++){
                for(int maxY = 1; maxY <= response->GetNbinsY(); maxY++){
                    response->SetBinContent(maxX,maxY,0);
                }
            }
            for(int maxX = 1; maxX <= response->GetNbinsX(); maxX++){
                for(int maxY = response->GetYaxis()->FindBin(ymax+1.); maxY <= response->GetNbinsY(); maxY++){
                    response->SetBinContent(maxX,maxY,0);
                }
            }
            //for(int minX = 1; minX <= 10; minX++){
            //    for(int minY = 1; minY <= response->GetNbinsY(); minY++){
            //        response->SetBinContent(minX,minY,0);
            //    }
            //}

            responseClosure->SetNameTitle(Form("Rawresponse_R0%i", radius), Form("Raw response R=0.%i", radius));
            responseClosure->SetName(Form("%s_fine", responseClosure->GetName()));
            auto rebinnedresponseclosure = makeRebinned2D(responseClosure, binningdet, binningpart);
            rebinnedresponseclosure->SetName(Form("%s_closure", rebinnedresponseclosure->GetName()));

            for(int x = 1; x <= rebinnedresponseclosure->GetNbinsX(); x++){
                for(int y = 1; y <= rebinnedresponseclosure->GetNbinsY(); y++){
                    float lowXval = rebinnedresponseclosure->GetXaxis()->GetBinLowEdge(x);
                    float lowYval = rebinnedresponseclosure->GetYaxis()->GetBinLowEdge(y);
                    float upXval  = rebinnedresponseclosure->GetXaxis()->GetBinUpEdge(x);
                    float upYval  = rebinnedresponseclosure->GetYaxis()->GetBinUpEdge(y);

                    int lowXbin = responseClosure->GetXaxis()->FindBin(lowXval);
                    int lowYbin = responseClosure->GetYaxis()->FindBin(lowYval);
                    int upXbin = responseClosure->GetXaxis()->FindBin(upXval);
                    int upYbin = responseClosure->GetYaxis()->FindBin(upYval);

                    double content = rebinnedresponseclosure->GetBinContent(x,y);
                    if(content < 10){
                        rebinnedresponseclosure->SetBinContent(x,y,0.);
                        for(int finebinX = lowXbin; finebinX <= upXbin; finebinX++){
                            for(int finebinY = lowYbin; finebinY <= upYbin; finebinY++){
                                responseClosure->SetBinContent(finebinX,finebinY,0);
                            }
                        }
                    }
                }
            }

            for(int maxX = responseClosure->GetXaxis()->FindBin(xmax+1.); maxX <= responseClosure->GetNbinsX(); maxX++){
                for(int maxY = 1; maxY <= responseClosure->GetNbinsY(); maxY++){
                    responseClosure->SetBinContent(maxX,maxY,0);
                }
            }
            for(int maxX = 1; maxX <= responseClosure->GetNbinsX(); maxX++){
                for(int maxY = responseClosure->GetYaxis()->FindBin(ymax+1.); maxY <= responseClosure->GetNbinsY(); maxY++){
                    responseClosure->SetBinContent(maxX,maxY,0);
                }
            }
            //for(int minX = 1; minX <= 10; minX++){
            //    for(int minY = 1; minY <= responseClosure->GetNbinsY(); minY++){
            //        responseClosure->SetBinContent(minX,minY,0);
            //    }
            //}

            responseNoClosure->SetNameTitle(Form("Rawresponse_R0%i", radius), Form("Raw response R=0.%i", radius));
            responseNoClosure->SetName("truthclosure");
            auto rebinnedresponsenoclosure = makeRebinned2D(responseNoClosure, binningdet, binningpart);
            rebinnedresponsenoclosure->SetName(Form("%s_noclosure", rebinnedresponsenoclosure->GetName()));

            for(int x = 1; x <= rebinnedresponse->GetNbinsX(); x++){
                for(int y = 1; y <= rebinnedresponsenoclosure->GetNbinsY(); y++){
                    float lowXval = rebinnedresponsenoclosure->GetXaxis()->GetBinLowEdge(x);
                    float lowYval = rebinnedresponsenoclosure->GetYaxis()->GetBinLowEdge(y);
                    float upXval  = rebinnedresponsenoclosure->GetXaxis()->GetBinUpEdge(x);
                    float upYval  = rebinnedresponsenoclosure->GetYaxis()->GetBinUpEdge(y);

                    int lowXbin = responseNoClosure->GetXaxis()->FindBin(lowXval);
                    int lowYbin = responseNoClosure->GetYaxis()->FindBin(lowYval);
                    int upXbin = responseNoClosure->GetXaxis()->FindBin(upXval);
                    int upYbin = responseNoClosure->GetYaxis()->FindBin(upYval);

                    double content = rebinnedresponsenoclosure->GetBinContent(x,y);
                    if(content < 10){
                        rebinnedresponsenoclosure->SetBinContent(x,y,0.);
                        for(int finebinX = lowXbin; finebinX <= upXbin; finebinX++){
                            for(int finebinY = lowYbin; finebinY <= upYbin; finebinY++){
                                responseNoClosure->SetBinContent(finebinX,finebinY,0);
                            }
                        }
                    }
                }
            }

            for(int maxX = responseNoClosure->GetXaxis()->FindBin(xmax+1.); maxX <= responseNoClosure->GetNbinsX(); maxX++){
                for(int maxY = 1; maxY <= responseNoClosure->GetNbinsY(); maxY++){
                    responseNoClosure->SetBinContent(maxX,maxY,0);
                }
            }
            for(int maxX = 1; maxX <= responseNoClosure->GetNbinsX(); maxX++){
                for(int maxY = responseNoClosure->GetYaxis()->FindBin(ymax+1.); maxY <= responseNoClosure->GetNbinsY(); maxY++){
                    responseNoClosure->SetBinContent(maxX,maxY,0);
                }
            }
            //for(int minX = 1; minX <= 10; minX++){
            //    for(int minY = 1; minY <= responseNoClosure->GetNbinsY(); minY++){
            //        responseNoClosure->SetBinContent(minX,minY,0);
            //    }
            //}

            rebinnedresponse->Scale(mcscale);
            rebinnedresponseclosure->Scale(mcscale);
            rebinnedresponsenoclosure->Scale(mcscale);
            responseClosure->Scale(mcscale);
            responseNoClosure->Scale(mcscale);
            response->Scale(mcscale);

            if(bin==1){
                arrResponse[radius-minradius] = response;
                arrResponseRebinned[radius-minradius] = rebinnedresponse;
                arrResponseClosure[radius-minradius] = responseClosure;
                arrResponseClosureRebinned[radius-minradius] = rebinnedresponseclosure;
                arrResponseNoClosure[radius-minradius] = responseNoClosure;
                arrResponseNoClosureRebinned[radius-minradius] = rebinnedresponsenoclosure;

            }else{
                arrResponse[radius-minradius]->Add(response);
                arrResponseRebinned[radius-minradius]->Add(rebinnedresponse);
                arrResponseClosure[radius-minradius]->Add(responseClosure);
                arrResponseClosureRebinned[radius-minradius]->Add(rebinnedresponseclosure);
                arrResponseNoClosure[radius-minradius]->Add(responseNoClosure);
                arrResponseNoClosureRebinned[radius-minradius]->Add(rebinnedresponsenoclosure);
            }
        }
    }
    const char* nameOutput = Form("%s/filteredResponse.root",fout.Data());
    TFile* fOutput = new TFile(nameOutput,"RECREATE");
    for(int radius = minradius; radius <= maxradius; radius++){
        fOutput->mkdir(Form("R0%i",radius));
        fOutput->cd(Form("R0%i",radius));
        arrResponse[radius-minradius]->Write();
        arrResponseRebinned[radius-minradius]->Write();
        arrResponseClosure[radius-minradius]->Write();
        arrResponseClosureRebinned[radius-minradius]->Write();
        arrResponseNoClosure[radius-minradius]->Write();
        arrResponseNoClosureRebinned[radius-minradius]->Write();

        for(int xbin = 1; xbin <= arrResponseRebinned[radius-minradius]->GetNbinsX(); xbin++){
            for(int ybin = 1; ybin <= arrResponseRebinned[radius-minradius]->GetNbinsY(); ybin++){
                float lowXval = arrResponseRebinned[radius-minradius]->GetXaxis()->GetBinLowEdge(xbin);
                float lowYval = arrResponseRebinned[radius-minradius]->GetYaxis()->GetBinLowEdge(ybin);
                float upXval  = arrResponseRebinned[radius-minradius]->GetXaxis()->GetBinUpEdge(xbin);
                float upYval  = arrResponseRebinned[radius-minradius]->GetYaxis()->GetBinUpEdge(ybin);

                double content = arrResponseRebinned[radius-minradius]->GetBinContent(xbin,ybin);

                //if(content < 10 && content > 0){
                //        cout << "Radius = 0." << radius << endl;
                //        cout << "pT Det: " << lowXval << " - " << upXval << endl;
                //        cout << "pT Part: " << lowYval << " - " << upYval << endl;
                //        cout << "Content: " << content << endl << endl;
                //}

            }
        }
    }
    fOutput->Close();
    delete fOutput;

    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.1,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogz();

    for(int radius = minradius; radius <= maxradius; radius++){
        TH2 *drawresponse = (TH2*)arrResponse[radius-minradius]->Clone(Form("drawresponse_R0%i",radius));
        //drawresponse->GetXaxis()->SetRangeUser(0,200);
        //drawresponse->GetYaxis()->SetRangeUser(0,300);
        drawresponse->GetZaxis()->SetRangeUser(drawresponse->GetMinimum(0.),drawresponse->GetMaximum());
        SetStyleHistoTH2ForGraphs(drawresponse,"#it{p}_{T}^{det} (GeV/#it{c})","#it{p}_{T}^{part}",textSize,0.04,textSize,0.04,1,1.2);
        drawresponse->Draw("colz");
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
        c->SaveAs(Form("%s/Standard/Response_R0%i.%s",fout.Data(),radius,fileType.Data()));

        TH2 *drawresponseclosure = (TH2*)arrResponseClosure[radius-minradius]->Clone(Form("drawresponseclosure_R0%i",radius));
        //drawresponse->GetXaxis()->SetRangeUser(0,200);
        //drawresponse->GetYaxis()->SetRangeUser(0,300);
        drawresponseclosure->GetZaxis()->SetRangeUser(drawresponseclosure->GetMinimum(0.),drawresponseclosure->GetMaximum());
        SetStyleHistoTH2ForGraphs(drawresponseclosure,"#it{p}_{T}^{det} (GeV/#it{c})","#it{p}_{T}^{part}",textSize,0.04,textSize,0.04,1,1.2);
        drawresponseclosure->Draw("colz");
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
        c->SaveAs(Form("%s/Closure/ResponseClosure_R0%i.%s",fout.Data(),radius,fileType.Data()));

        TH2 *drawresponsenoclosure = (TH2*)arrResponseNoClosure[radius-minradius]->Clone(Form("drawresponsenoclosure_R0%i",radius));
        //drawresponse->GetXaxis()->SetRangeUser(0,200);
        //drawresponse->GetYaxis()->SetRangeUser(0,300);
        drawresponsenoclosure->GetZaxis()->SetRangeUser(drawresponsenoclosure->GetMinimum(0.),drawresponsenoclosure->GetMaximum());
        SetStyleHistoTH2ForGraphs(drawresponsenoclosure,"#it{p}_{T}^{det} (GeV/#it{c})","#it{p}_{T}^{part}",textSize,0.04,textSize,0.04,1,1.2);
        drawresponsenoclosure->Draw("colz");
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
        c->SaveAs(Form("%s/TruthClosure/ResponseTruthClosure_R0%i.%s",fout.Data(),radius,fileType.Data()));
    }
    for(int radius = minradius; radius <= maxradius; radius++){
        TH2 *drawresponse = (TH2*)arrResponseRebinned[radius-minradius]->Clone(Form("drawresponse_R0%i",radius));
        //drawresponse->GetXaxis()->SetRangeUser(0,200);
        //drawresponse->GetYaxis()->SetRangeUser(0,300);
        drawresponse->GetZaxis()->SetRangeUser(drawresponse->GetMinimum(0.),drawresponse->GetMaximum());
        SetStyleHistoTH2ForGraphs(drawresponse,"#it{p}_{T}^{det} (GeV/#it{c})","#it{p}_{T}^{part}",textSize,0.04,textSize,0.04,1,1.2);
        drawresponse->Draw("colz");
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
        c->SaveAs(Form("%s/Standard/ResponseRebinned_R0%i.%s",fout.Data(),radius,fileType.Data()));

        TH2 *drawresponseclosure = (TH2*)arrResponseClosureRebinned[radius-minradius]->Clone(Form("drawresponseclosure_R0%i",radius));
        //drawresponse->GetXaxis()->SetRangeUser(0,200);
        //drawresponse->GetYaxis()->SetRangeUser(0,300);
        drawresponseclosure->GetZaxis()->SetRangeUser(drawresponseclosure->GetMinimum(0.),drawresponseclosure->GetMaximum());
        SetStyleHistoTH2ForGraphs(drawresponseclosure,"#it{p}_{T}^{det} (GeV/#it{c})","#it{p}_{T}^{part}",textSize,0.04,textSize,0.04,1,1.2);
        drawresponseclosure->Draw("colz");
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
        c->SaveAs(Form("%s/Closure/ResponseClosureRebinned_R0%i.%s",fout.Data(),radius,fileType.Data()));

        TH2 *drawresponsenoclosure = (TH2*)arrResponseNoClosureRebinned[radius-minradius]->Clone(Form("drawresponsenoclosure_R0%i",radius));
        //drawresponse->GetXaxis()->SetRangeUser(0,200);
        //drawresponse->GetYaxis()->SetRangeUser(0,300);
        drawresponsenoclosure->GetZaxis()->SetRangeUser(drawresponsenoclosure->GetMinimum(0.),drawresponsenoclosure->GetMaximum());
        SetStyleHistoTH2ForGraphs(drawresponsenoclosure,"#it{p}_{T}^{det} (GeV/#it{c})","#it{p}_{T}^{part}",textSize,0.04,textSize,0.04,1,1.2);
        drawresponsenoclosure->Draw("colz");
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
        c->SaveAs(Form("%s/TruthClosure/ResponseTruthClosureRebinned_R0%i.%s",fout.Data(),radius,fileType.Data()));
    }
}
