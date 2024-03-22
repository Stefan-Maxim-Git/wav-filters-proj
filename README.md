# wav-filters-proj

<h2>Description</h2>
An application built in LabWindows/CVI that allows for applying different types of filters on an audio file and visualizing the results.

<h2> Requirements </h2>

This project was create using the following:

<ul>
  <li><a href="https://www.ni.com/en/support/downloads/software-products/download.labwindows-cvi.html#353603" target="_blank" rel="noopener noreferrer">LabWindows/CVI</a></li>
</ul>

<h2>Functionality</h2>
The project's scope is to load the data extracted from a .wav file (done using a Python script in Google Colab) and apply different types of signal filters and windows. The signal can be processed in both time and frequency domains.

<h3> Time domain </h3>
In the time domain, the signal is represented on a graph and can go up to 6 seconds of length. The interface allows for the visualization of time frames that span for 1 second, allowing the user to move forward and backward by 1 second for a closer analysis. The interface also displays other useful data such as kurtosis, skewness, and histograms of the signal array representation. <br/>
The time domain analysis disposes of two filters which can be applied: 

<ul>
  <li>Alpha Filter: FIR filter used for smoothing the signal. The alpha parameter can be changed ((0, 1) interval). </li>
  <li>Simple Moving Average Filter: FIR filter used for smoothing the signal. The user has the option to select the number of samples selected for calculating the mean (16 or 32 samples).</li>
</ul>

<h3> Frequency domain </h3>
In the frequency domain, the spectrum of the signal is represented on a graph using a clock. For each clock tick, the program calculates and displays the spectrum for the signal, The frequency domain representation disposes of two windows (Welch and Gaussian) and one filter (4th order Butterworth bandpass filter for frequencies between 400 - 1100 Hz range). 
