#include "/home/austin/alice/SubstructureAnalysis/meta/stl.C"
#include "/home/austin/alice/SubstructureAnalysis/meta/root.C"
#include "/home/austin/alice/SubstructureAnalysis/helpers/math.C"
#include "/home/austin/alice/SubstructureAnalysis/helpers/unfolding.C"

std::tuple<double, double, TH1 *> makeRejectionFactor(TH1 *histhigh, TH1 *histlow, int R, const std::string_view triggerlow, const std::string_view triggerhigh, const std::string_view sysvar, double linlow, double linhigh, double eflow, double efhigh, bool fUseErrorFunction = true){
    auto rfac = histcopy(histhigh);
    rfac->SetNameTitle(Form("RejectionFactor_%s_%s_R%02d", triggerhigh.data(), triggerlow.data(), R), Form("Rejection Factor for %s/%s for R=%.1f", triggerhigh.data(), triggerlow.data(), double(R)/10.));
    rfac->SetDirectory(nullptr);
    rfac->Divide(histhigh, histlow, 1., 1.);

    double triggerRF, triggerTF_Error;
    TF1* pol0 = new TF1("pol0", "[0]", linlow, linhigh);
    if(fUseErrorFunction) rfac->Fit(pol0, "QNRMEX0+", "", linlow, linhigh);
    else rfac->Fit(pol0, "QRMEX+", "", linlow, linhigh);
    TF1* erfunc = new TF1("erfunc" ,"[3]+[2] * TMath::Erf( (x-[0])/(TMath::Sqrt(2)*[1]))", eflow, efhigh);

    if(fUseErrorFunction){
        erfunc->SetParameter(0,4.);
        erfunc->SetParameter(1,1.);
        erfunc->SetParameter(2,pol0->GetParameter(0)/2);
        erfunc->SetParameter(3,pol0->GetParameter(0)/2);
        rfac->Fit(erfunc, "QRMEX+", "", eflow, efhigh);
        triggerRF = erfunc->GetParameter(2)+erfunc->GetParameter(3);
        triggerTF_Error = (erfunc->GetParameter(2)+erfunc->GetParameter(3))*TMath::Sqrt(TMath::Power(erfunc->GetParError(2)/erfunc->GetParameter(2),2)+TMath::Power(erfunc->GetParError(3)/erfunc->GetParameter(3),2));
    }else{
        triggerRF = pol0->GetParameter(0);
        triggerTF_Error = pol0->GetParError(0);
    }

    return {triggerRF, triggerTF_Error, rfac};
}
