import ROOT as r 
import copy

evts = {
    'nu0':   492000,
    'nu140': 496400,
    'nu200': 500000,
}

r.gStyle.SetOptStat(0)

def getPlotFromSample( h, sample):

    cum = h.GetCumulative()
    cum2 = copy.deepcopy(cum)
    for i in range(1,cum.GetNbinsX()+1):
        cum.SetBinContent( i, h.Integral()-cum2.GetBinContent(i-1))
        print cum.GetBinContent(i)

    cum.Scale(40*1e3/evts[sample])
    return cum


def makeRatePlots():

    mu140 = r.TFile.Open('omtfAnalysis_SingleNu140.root')
    var140 = mu140.Get('hRate').Clone('mu140')
    var140 = copy.deepcopy( var140 )
    mu140.Close()
    
    mu200 = r.TFile.Open('omtfAnalysis_SingleNu200.root')
    var200 = mu200.Get('hRate').Clone('mu200')
    var200 = copy.deepcopy( var200 )
    mu200.Close()

    mu0 = r.TFile.Open('omtfAnalysis_SingleNuNoPU.root')
    var0 = mu0.Get('hRate').Clone('mu0')
    var0 = copy.deepcopy( var0 )
    mu0.Close()

    #plot0 = getPlotFromSample( var0, 'nu0')
    plot140 = getPlotFromSample( var140, 'nu140')
    plot200 = getPlotFromSample( var200, 'nu200')

    plot200.SetTitle(';p_{T} [GeV];Rate [kHz]') 
    plot140.SetLineColor(r.kRed) ; plot140.SetLineWidth(2)
    plot200.SetLineColor(r.kBlue); plot200.SetLineWidth(2)
    #plot0.SetLineColor(r.kBlue)  ; plot0  .SetLineWidth(2) 
    
    c = r.TCanvas()

    plot200.Draw()
    plot200.GetYaxis().SetRangeUser(1e-1,5e3)
    plot140.Draw('same')
    #plot0  .Draw('same')

    leg = r.TLegend(0.6,0.6,0.75,0.7)
    leg.AddEntry(plot140,'PU140','l');
    leg.AddEntry(plot200,'PU200','l');
    #leg.AddEntry(plot0,'No pileup','l'); 
    leg.Draw('same')

    c.SetLogy()

    c.SaveAs('rate.png' )
    c.SaveAs('rate.pdf' )
    c.SaveAs('rate.root')
    
    kk = r.TRatioPlot(plot200, plot140)
    kk.Draw()
    c.SaveAs('rate_withratio.png')
    

def makeEffPlots():


    for var in 'hEffPt,hEffEta'.split(','):
        mu140 = r.TFile.Open('omtfAnalysis_SingleMu140.root')
        var140 = mu140.Get(var).Clone(var+'mu140')
        var140 = copy.deepcopy( var140 )
        mu140.Close()
        
        mu200 = r.TFile.Open('omtfAnalysis_SingleMu200.root')
        var200 = mu200.Get(var).Clone(var+'mu140')
        var200 = copy.deepcopy( var200 )
        mu200.Close()


        mu0 = r.TFile.Open('omtfAnalysis_SingleMuNoPU.root')
        var0 = mu0.Get(var).Clone(var+'mu140')
        var0 = copy.deepcopy( var0 )
        mu0.Close()


        var0.SetLineColor(r.kBlack) ; var0.SetLineWidth(2)
        var140.SetLineColor(r.kRed) ; var140.SetLineWidth(2)
        var200.SetLineColor(r.kBlue); var200.SetLineWidth(2)

        c = r.TCanvas()
        
        var140.SetTitle(';{xaxis};{yaxis}'.format(xaxis='p_{T} [GeV]' if 'Pt' in var else '#eta',yaxis='Efficiency'))
        var140.Draw()
        var200.Draw('same')
        var0.Draw('same')

        leg = r.TLegend(0.6,0.3,0.75,0.4)
        leg.AddEntry(var140,'PU140','l');
        leg.AddEntry(var200,'PU200','l'); 
        leg.AddEntry(var0, 'No pile up', 'l');
        leg.Draw('same')
        
        c.SaveAs('eff_%s.png'%var)
        c.SaveAs('eff_%s.pdf'%var)
        c.SaveAs('eff_%s.root'%var)
        
        


#makeEffPlots()
makeRatePlots()
