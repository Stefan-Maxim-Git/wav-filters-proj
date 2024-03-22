#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Interfata.h"
#include "Histograms.h"
#include <analysis.h>

#define INTERVAL 44100

// LUCRURI PE CARE INCA NU LE-AM FACUT (sorry):
	// Kurtosis si Skewness (am aflat cum dar nu am adaugat inca controale pe interfata pentru ele) -> check getInfo
	// Anvelopa
	// Salvare poze cu grafice
// Le termin pana la urmatoarea sedinta de laborator :DDD

//----------------------------------> Variabile Globale <-----------------------------
typedef struct {
	double max;
	ssize_t i_max;
	double min;
	ssize_t i_min;
	double mean;
	double dispersion;
	ssize_t zero_pass;
	double skewness;
	double kurtosis;
} Info;

static int mainPanel;
static int histPanel;
static int freqPanel;

int waveInfo[2];
double* waveData;
double* filteredData;
double* envelope;

double sampleRate;
int npoints;

Info rawinfo;
Info filteredInfo;

int filterType;
double alpha = 0.655;
int median_nr = 16;

int left = 0, right = 1;

//-----------------------------------> Functii <-----------------------------------------


double getStdDev (double* data) {
	double stdDev;
	double mean;
	
	Mean(data, npoints, &mean);
	StdDev(data, (ssize_t)npoints, &mean, &stdDev);
	
	return stdDev;
}

double* getIntervalData(double* waveData, int left, int right) {
	
	if (waveData == NULL) {
		return NULL;
	}
	size_t allocSize = (right - left) * INTERVAL;
	double* plotData = (double*) calloc(allocSize, sizeof(double));
	int i;
	
	for (i = left * INTERVAL; i < right * INTERVAL; i++) {
		plotData[i - left * INTERVAL] = waveData[i];
	}
	
	return plotData;
}

void updIntervalDisplay(int panel) {
	SetCtrlVal(panel, MAIN_PANEL_BOX_LEFT_INT, left);
	SetCtrlVal(panel, MAIN_PANEL_BOX_RIGHT_INT, right);
}

double* getFilteredData(double* waveData, int filterType) {
	if (waveData == NULL) {
		return NULL;
	}
	
	double* filteredData = (double*) calloc (npoints, sizeof(double));
	int i;
	double sum = 0;
	switch (filterType) {
		case 0:
			free(filteredData);
			filteredData = NULL;
			break;
		case 1:
			filteredData[0] = waveData[0];
			for (i = 1; i < npoints; i++) {
				filteredData[i] = (1 - alpha) * filteredData[i - 1]	+ alpha * waveData[i];		
			}
			break;
		case 2:
			for (i = 0; i < median_nr; i++) {
				sum += waveData[i];
			}
			filteredData[0] = sum / median_nr;
			
			for (i = 1; i < npoints - median_nr + 1; i++) {
				sum = sum - waveData[i - 1] + waveData[median_nr + i - 1];
				filteredData[i] = sum / median_nr;
			}
 			break;
	}
	
	return filteredData;
}

// Computes and plots the histograms for raw and filtered data
void UpdateHistogram(double* data, int dataType) {
	double max, min;
 	ssize_t imax, imin;
	ssize_t intervals = 20; // Using square root formula for number of bins
	//double binWidth = 3.5 * (getStdDev(data) / pow(npoints, 1.0/3));
	
	
	int* histData = (int*) calloc(20, sizeof(int) );
	double* histAxis = (double*) calloc (20, sizeof(double));
	
	MaxMin1D(data, npoints, &max, &imax, &min, &imin);
	
	//int intervals = (int) ((max - min) / binWidth);
	
	Histogram(data, (ssize_t)npoints, min, max, histData, histAxis, intervals); 
	
	// Plotting:
	switch(dataType) {
		case 1:		// For raw data
			PlotXY(histPanel, HIST_PANEL_HIST_RAW, histAxis, histData, intervals, VAL_DOUBLE, VAL_INTEGER, VAL_VERTICAL_BAR, VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
			break;
		case 2:		//For filtered data
			PlotXY(histPanel, HIST_PANEL_HIST_FILTERED, histAxis, histData, intervals, VAL_DOUBLE, VAL_INTEGER, VAL_VERTICAL_BAR, VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
			break;
	}
	
}

int showInterval(int left, int right) {
			filteredData = getFilteredData(waveData, filterType);
			size_t plotInterval = (right - left) * INTERVAL;
			if (filteredData == NULL) {
				MessagePopup("Error", "Please select a filter...");
				return -1;
			}
			double* plotDataRaw = getIntervalData(waveData, left, right);
			double* plotDataFilter = getIntervalData(filteredData, left, right);
			
			if (plotDataRaw == NULL) {
				MessagePopup("Error", "Error getting plotting data. Wave Data not loaded."); 
				return -1;
			}
			
			if (plotDataFilter == NULL) {
				MessagePopup("Error", "Error getting plotting data. Filter not applied."); 
				return -1;
			}
			
			// Kurtosis and skewness for the first 256 points of an interval:
			// Doing this here since I already have the plot data prepared for specific intervals
			
			Moment(plotDataFilter, 256, 3, &filteredInfo.skewness); // Skewness
			Moment(plotDataFilter, 256, 4, &filteredInfo.kurtosis);	// Kurtosis
			
			SetCtrlAttribute(mainPanel, MAIN_PANEL_TEXT_KURT, ATTR_VISIBLE, 1);
			SetCtrlAttribute(mainPanel, MAIN_PANEL_TEXT_SKEW, ATTR_VISIBLE, 1);
			
			if (filteredInfo.kurtosis == 0.0)
				SetCtrlVal(mainPanel, MAIN_PANEL_TEXT_KURT, "Kurtosis: mezokurtic");
			else if (filteredInfo.kurtosis > 0.0)
				SetCtrlVal(mainPanel, MAIN_PANEL_TEXT_KURT, "Kurtosis: leptokurtic");
			else if (filteredInfo.kurtosis < 0.0)
				SetCtrlVal(mainPanel, MAIN_PANEL_TEXT_KURT, "Kurtosis: platikurtic");
			
			if (filteredInfo.skewness == 0.0)
				SetCtrlVal(mainPanel, MAIN_PANEL_TEXT_SKEW, "Skewness: symmetric");
			else if (filteredInfo.skewness < 0.0)
				SetCtrlVal(mainPanel, MAIN_PANEL_TEXT_SKEW, "Skewness: negative skew");
			else if (filteredInfo.skewness > 0.0)
				SetCtrlVal(mainPanel, MAIN_PANEL_TEXT_SKEW, "Skewness: positive skew");
			
			DeleteGraphPlot(mainPanel, MAIN_PANEL_GRAPH_RAW, -1, VAL_IMMEDIATE_DRAW);
			PlotY(mainPanel, MAIN_PANEL_GRAPH_RAW, plotDataRaw, plotInterval, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			DeleteGraphPlot(mainPanel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
			PlotY(mainPanel, MAIN_PANEL_GRAPH_FILTERED, plotDataFilter, plotInterval, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			return 0;
}


	
//------------------------------------> Main <----------------------------------------------//


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "Interfata.uir", MAIN_PANEL)) < 0)
		return -1;
	if ((histPanel = LoadPanel (1, "Histograms.uir", HIST_PANEL)) < 0)
		return -1;
	if ((freqPanel = LoadPanel(0, "Interfata.uir", FREQ_PANEL)) < 0)
		return -1;
	// Valori initiale pentru filtre:
	SetCtrlVal(mainPanel, MAIN_PANEL_BOX_MEDIAN, median_nr);
	SetCtrlVal(mainPanel, MAIN_PANEL_BOX_ALPHA, alpha);
	//--------------------------------
	
	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	return 0;
}

//-------------------------------------> Callback Functions <------------------------------------//

int CVICALLBACK OnLoadButton (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			FileToArray("waveInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[0];
			npoints = waveInfo[1];
			
			
			waveData = (double*) calloc(npoints, sizeof(double));
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			PlotY(panel, MAIN_PANEL_GRAPH_RAW, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			SetCtrlAttribute(mainPanel, MAIN_PANEL_BTN_LOAD, ATTR_VISIBLE, 0);
			break;
	}
	return 0;
}

int CVICALLBACK OnPanel (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK UpdateGraph (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	int retVal = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			retVal = showInterval(left, right);
			break;
	}
	return retVal;
}

int CVICALLBACK nextInt (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{	
	int retVal = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			if (right < 6) {
				left++;
				right++;
				updIntervalDisplay(panel);
				retVal = showInterval(left, right);
			}
			break;
	}
	return retVal;
}

int CVICALLBACK prevInt (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int retVal = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			if (left > 0) {
				left--;
				right--;
				updIntervalDisplay(panel);
				retVal = showInterval(left, right);
			}
			break;
	}
	return retVal;
}

int CVICALLBACK getFilterType (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, MAIN_PANEL_MENU_FILTERS, &filterType);
			SetCtrlAttribute(panel, MAIN_PANEL_BOX_ALPHA, ATTR_VISIBLE, filterType == 1);
			SetCtrlAttribute(panel, MAIN_PANEL_BOX_MEDIAN, ATTR_VISIBLE, filterType == 2);
			break;
	}
	return 0;
}

int CVICALLBACK getAlphaValue (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, MAIN_PANEL_BOX_ALPHA, &alpha);
			break;
	}
	return 0;
}

int CVICALLBACK getElemForMedian (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, MAIN_PANEL_BOX_MEDIAN, &median_nr);
			break;
	}
	return 0;
}

int CVICALLBACK showHist (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	//static BOOL visible = 0;
	int visible;
	switch (event)
	{
		case EVENT_COMMIT:
			GetPanelAttribute(histPanel, ATTR_VISIBLE, &visible);
			UpdateHistogram(filteredData, 2);
			UpdateHistogram(waveData, 1);
			switch (visible) {
				case 0: DisplayPanel(histPanel);
				break;
				case 1: HidePanel(histPanel);
				break;
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK closeHistPanel (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{	int visible;
	switch (event)
	{
		case EVENT_COMMIT:
			GetPanelAttribute(histPanel, ATTR_VISIBLE, &visible);
			if (visible) HidePanel(histPanel);
			break;
	}
	return 0;
}

int CVICALLBACK getInfo (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (waveData == NULL) {
				MessagePopup("Error", "Wave data not loaded, please load data first!");
				return -1;
			} else {
				// For max and min of waveData:
				MaxMin1D(waveData, npoints, &rawinfo.max, &rawinfo.i_max, &rawinfo.min, &rawinfo.i_min);
				SetCtrlVal(histPanel, HIST_PANEL_BOX_RAW_MAX, rawinfo.max);
				SetCtrlVal(histPanel, HIST_PANEL_BOX_RAW_MIN, rawinfo.min);
				
				//For mean and dispersion of waveData:
				
				Mean(waveData, npoints, &rawinfo.mean);
				SetCtrlVal(histPanel, HIST_PANEL_BOX_RAW_MEAN, rawinfo.mean);
				
				// DO DISPERSION LATER!!!!
				
				// Passings through zero:
				rawinfo.zero_pass = 0;
			
				
				int i;
				for (i = 0; i < npoints - 1; i++) {
					if (waveData[i] < 0 && waveData[i + 1] > 0)
						rawinfo.zero_pass++;
				}
				
				SetCtrlVal(histPanel, HIST_PANEL_BOX_RAW_ZERO, rawinfo.zero_pass);
			}
				   
			if (filteredData == NULL) {
				MessagePopup("Error", "No filter applied to data, please select a filter and apply it!");
				//return -1;
			} else {
				//For max and min of filteredData
				MaxMin1D(filteredData, npoints, &filteredInfo.max, &filteredInfo.i_max, &filteredInfo.min, &filteredInfo.i_min);
				SetCtrlVal(histPanel, HIST_PANEL_BOX_FILTERED_MAX, filteredInfo.max);
				SetCtrlVal(histPanel, HIST_PANEL_BOX_FILTERED_MIN, filteredInfo.min);
				
				//For mean and dispersion of filteredData:
				
				Mean(filteredData, npoints, &filteredInfo.mean);
				SetCtrlVal(histPanel, HIST_PANEL_BOX_FILTERED_MEAN, filteredInfo.mean);
				
				// Passings through zero:
				filteredInfo.zero_pass = 0;
				
				int i;
				
				for (i = 0; i < npoints - 1; i++) {
					if (filteredData[i] < 0 && waveData[i + 1] > 0)
						filteredInfo.zero_pass++;
				}
				
				SetCtrlVal(histPanel, HIST_PANEL_BOX_FILTERED_ZERO, filteredInfo.zero_pass);
			}
 			break;
	}
	return 0;
}

int CVICALLBACK saveData (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK displayFullData (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{	
	int retVal = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			retVal = showInterval(1, 6);
			break;
	}
	return retVal;
}

int CVICALLBACK showEnvelope (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			envelope = (double*) calloc (npoints, sizeof(double));
			FileToArray("envelopeData.txt", envelope, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			PlotY(mainPanel, MAIN_PANEL_GRAPH_RAW, envelope, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_CYAN);
			break;
	}
	return 0;
}

int CVICALLBACK SwitchToFreq (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (panel == mainPanel) {
				SetCtrlVal(mainPanel, MAIN_PANEL_SW_FREQ, 0);
				DisplayPanel(freqPanel);
				HidePanel(mainPanel);
			} else if (panel == freqPanel) {
				SetCtrlVal(freqPanel, FREQ_PANEL_SW_FREQ, 1);
				DisplayPanel(mainPanel);
				HidePanel(freqPanel);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnFreqPanelCB (int panel, int event, void *callbackData,
							   int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface(0);
	return 0;
}


//--------------------------------> Etapa 2 <--------------------------------------

int npoints_freq = 1024;


// struct approach doesn't seem to work, doing brute force
typedef struct {
	double* plotData;
	double* autoSpectrum;
	double df, freqPeak, powerPeak;
	WindowConst winConst;
} Frequency;

//-------------- ignore ----------
Frequency RawNoDS = {NULL, NULL};
Frequency FilteredNoDs;
Frequency RawDS;
Frequency FilteredDS;
//-------------- ignore -------------

double* RNDSplotData = NULL;
double* RNDSautoSpectrum = NULL;
double RNDSdf;
double RNDSfreqPeak, RNDSpowerPeak;
WindowConst RNDSwinConst;

double* RDSplotData = NULL;
double* RDSautoSpectrum = NULL;
double RDSdf;
double RDSfreqPeak, RDSpowerPeak;
WindowConst RDSwinConst;

double* FNDSplotData = NULL;
double* FNDSautoSpectrum = NULL;
double FNDSdf, FNDSfreqPeak, FNDSpowerPeak;
WindowConst FNDSwinConst;

int plotOffset = 0;
int windowTypeFreq = 0;
int filterTypeFreq = 0;

double* downsampledArray = NULL;
int downsampledSize;

void getParamsFreq() {
	GetCtrlVal(freqPanel, FREQ_PANEL_SPECTRE_FILTERS, &filterTypeFreq);
	GetCtrlVal(freqPanel, FREQ_PANEL_SPECTRE_WINDOWS, &windowTypeFreq);
}

double* downsample(double* data, int factor, int* newSize) {

	int i, j;
	*newSize = (INTERVAL * 6 + factor - 1) / factor; 
	double* output = (double*) malloc (sizeof(double) * (*newSize));
	
	if (data)
	for (i = 0, j = 0; i < INTERVAL * 6; i += factor, j++) {
        output[j] = data[i];
    }
	
	return output;
}
		
int CVICALLBACK getNpoints (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(freqPanel, FREQ_PANEL_CTRL_NUM_OF_POINTS, &npoints_freq);
			break;
	}
	return 0;
}

int CVICALLBACK updateSpecters (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			if (!downsampledArray && !waveData)
				downsampledArray = downsample(waveData, 4, &downsampledSize);
			
			if(RNDSplotData)
				free(RNDSplotData);
			if(RDSplotData)
				free(RDSplotData);
			if(FNDSplotData)
				free(FNDSplotData);
			
			RDSplotData = (double*) calloc (npoints_freq, sizeof(double));
			RNDSplotData = (double*) calloc (npoints_freq, sizeof(double));
			FNDSplotData = (double*) calloc (npoints_freq, sizeof(double));
			
			if(RNDSplotData && waveData) {
				plotOffset = plotOffset < (INTERVAL * 6) / npoints_freq ? plotOffset : 0;
				DeleteGraphPlot(freqPanel, FREQ_PANEL_RAW_NO_DS, -1, VAL_DELAYED_DRAW);
				memcpy(RNDSplotData, waveData + npoints_freq * plotOffset++,sizeof(double) * npoints_freq);
				
				getParamsFreq();
				
				switch (windowTypeFreq) {
					case 1:
						ScaledWindowEx(RNDSplotData, npoints_freq, WELCH, 0, &RNDSwinConst);
						break;
					case 2:
						ScaledWindowEx(RNDSplotData, npoints_freq, GAUSSIAN, -1, &RNDSwinConst);
						break;
					default:
						break;
				
				}
				
				RNDSautoSpectrum = (double*) calloc (npoints_freq/2, sizeof(double));
				AutoPowerSpectrum(RNDSplotData, npoints_freq, 1.0/sampleRate, RNDSautoSpectrum, &RNDSdf);
				PowerFrequencyEstimate(RNDSautoSpectrum, npoints_freq/2, -1, RNDSwinConst, RNDSdf, 7, &RNDSfreqPeak, &RNDSpowerPeak);
				PlotWaveform(freqPanel, FREQ_PANEL_RAW_NO_DS, RNDSautoSpectrum, npoints_freq/2, VAL_DOUBLE, 1.0, 0.0, 0.0, RNDSdf, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
				
				if(RNDSautoSpectrum)
					free(RNDSautoSpectrum);
				RNDSautoSpectrum = NULL;
			}
			
			if(FNDSplotData && waveData) {
				getParamsFreq();
				
				switch(filterTypeFreq) {
					case 1:
						break;
					case 2:
						Bw_BPF(waveData, npoints_freq, sampleRate, 400, 1100, 4, FNDSplotData);
						break;
					default:
						break;
				}
				
				plotOffset = plotOffset < (INTERVAL * 6) / npoints_freq ? plotOffset : 0;
				DeleteGraphPlot(freqPanel, FREQ_PANEL_FILT_NO_DS, -1, VAL_DELAYED_DRAW);
				memcpy(FNDSplotData, waveData + npoints_freq * plotOffset++,sizeof(double) * npoints_freq);
				
				switch (windowTypeFreq) {
					case 1:
						ScaledWindowEx(FNDSplotData, npoints_freq, WELCH, 0, &FNDSwinConst);
						break;
					case 2:
						ScaledWindowEx(FNDSplotData, npoints_freq, GAUSSIAN, -1, &FNDSwinConst);
						break;
					default:
						break;
				
				}
				
				FNDSautoSpectrum = (double*) calloc (npoints_freq/2, sizeof(double));
				AutoPowerSpectrum(FNDSplotData, npoints_freq, 1.0/sampleRate, FNDSautoSpectrum, &FNDSdf);
				PowerFrequencyEstimate(FNDSautoSpectrum, npoints_freq/2, -1, FNDSwinConst, FNDSdf, 7, &FNDSfreqPeak, &FNDSpowerPeak);
				PlotWaveform(freqPanel, FREQ_PANEL_FILT_NO_DS, FNDSautoSpectrum, npoints_freq/2, VAL_DOUBLE, 1.0, 0.0, 0.0, FNDSdf, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
				
				if(FNDSautoSpectrum)
					free(FNDSautoSpectrum);
				FNDSautoSpectrum = NULL;
			}
			
			if (RDSplotData && downsampledArray && waveData) {
				plotOffset = plotOffset < downsampledSize / npoints_freq ? plotOffset : 0; 
				DeleteGraphPlot(freqPanel, FREQ_PANEL_RAW_DS, -1, VAL_DELAYED_DRAW);
				memcpy(RDSplotData, waveData + npoints_freq * plotOffset++,sizeof(double) * npoints_freq);
				
				getParamsFreq();
				
				switch (windowTypeFreq) {
					case 1:
						ScaledWindowEx(RDSplotData, npoints_freq, WELCH, 0, &RNDSwinConst);
						break;
					case 2:
						ScaledWindowEx(RDSplotData, npoints_freq, GAUSSIAN, -1, &RNDSwinConst);
						break;
					default:
						break;
				
				}
				
				RDSautoSpectrum = (double*) calloc (npoints_freq/2, sizeof(double));
				AutoPowerSpectrum(RDSplotData, npoints_freq, 1.0/sampleRate, RDSautoSpectrum, &RDSdf);
				PowerFrequencyEstimate(RDSautoSpectrum, npoints_freq/2, -1, RDSwinConst, RDSdf, 7, &RDSfreqPeak, &RDSpowerPeak);
				PlotWaveform(freqPanel, FREQ_PANEL_RAW_DS, RDSautoSpectrum, npoints_freq/2, VAL_DOUBLE, 1.0, 0.0, 0.0, RDSdf, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
				
				if(RDSautoSpectrum)
					free(RDSautoSpectrum);
				RDSautoSpectrum = NULL;
			}
			break;
		}
	return 0;
}


