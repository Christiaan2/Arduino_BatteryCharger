%{
    Name: Christiaan Reurslag
    Studentnummer: s1495089
    Description: Plot the data from the Arduino in real Time and logs the 
                data in a txt file.
    MATLAB R2016b
%}

%Close all figures and clear workspace and command window
clear all
close all
clc

NChannels = 6;

%WindowSize = 10000*10*6; %Last 10 seconds are visible in figure
colours = {[0, 0.4470, 0.7410], [0.8500, 0.3250, 0.0980], [0.9290, 0.6940, 0.1250], ...
    [0.4940, 0.1840, 0.5560], 	[0.4660, 0.6740, 0.1880], 	[0.3010, 0.7450, 0.9330], ...
    [0.6350, 0.0780, 0.1840], [0.25, 0.25, 0.25]}; %Define the colors of the lines used to plot the data

Arduino=serial('COM4','BaudRate',115200,'TimeOut',inf);
fopen(Arduino); %Initiate arduino communication

%Create figure with labels and grid
figure('units','normalized','outerposition',[0 0.04 1 0.95]);
subplot(3,2,1)
title('Base Current (A)')
hold on
grid on
grid minor
%ylim([0,5]);%%%
xlabel('Time [ms]')
subplot(3,2,2)
title('Collector Current (A)')
hold on
grid on
grid minor
%ylim([0,5]);%%%
xlabel('Time [ms]')
subplot(3,2,3)
title('PWM signal')
hold on
grid on
grid minor
%ylim([0,5]);%%%
xlabel('Time [ms]')
subplot(3,2,4)
title('Input Voltage (V)')
hold on
grid on
grid minor
%ylim([0,5]);%%%
xlabel('Time [ms]')
subplot(3,2,5)
title('Temperature (graden C)')
hold on
grid on
grid minor
%ylim([0,5]);%%%
xlabel('Time [ms]')
subplot(3,2,6)
title('Battery voltage (V)')
hold on
grid on
grid minor
%ylim([0,5]);%%%
xlabel('Time [ms]')

%Open textfile
fileId = fopen('LogPlotData.txt','w');
fprintf(fileId,'Data recieved from Arduino\r\n');

j = 0;  %Count number of measurements
sampling = true;    %When true, Matlab looks for new data send from the Arduino

while(sampling)
    start_message = fread(Arduino,1,'uint16'); %Read data as integer (1 time 2 bytes (16 bits))
    if(start_message == 65535) %Start sign recieved from Arduino?
        j = j + 1;
        for k = 1:NChannels
            subplot(3,2,k)
            h(k) = animatedline;
            h(k).Color = colours{1}; %Set colour of line
        end
        
        fprintf(fileId,'\r\nMeasurement number: %u\r\n', j);
        for k = 1:NChannels
            fprintf(fileId,'\tValue%u',k);
        end
        fprintf(fileId,'\r\n');
        i = 0;  %Count number of samples taken
        SensorReading = zeros(1,NChannels,'uint16');
        disp('Start sampling')
        while(true)
            SensorReading(1) = fread(Arduino,1,'uint16');
            
            switch SensorReading(1)
                case 65279  %Stop measurement
                    disp('Stop sampling')
                    break
                case 65023  %Stop Matlab
                    sampling = false;
                    disp('Stop MATLAB')
                    break
            end
            
            for k = 2:NChannels
                SensorReading(k) = fread(Arduino,1,'uint16'); %Read data as integer (2 bytes)
            end
            Time = fread(Arduino,1,'uint32');%Read time as unsigned integer (4 bytes)
            
            SensorData = double(SensorReading) / 1024 * 5;
            plotData = zeros(1,NChannels);
            plotData(1) = (SensorData(1) - SensorData(2)) / 2160 - ((SensorData(2) - SensorData(4)) / 9870);
            plotData(2) = (SensorData(4) / 21.8) - plotData(1);
            plotData(3) = SensorData(3) * 1024 / 5;
            plotData(4) = SensorData(1);
            plotData(5) = (SensorData(6) - 0.5) * 100;
            plotData(6) = 12.10 - (SensorData(5) * 2);
            
            %Update texfile
            fprintf(fileId,'%u',Time);
            %Update graph
            for k = 1:NChannels
                addpoints(h(k),Time,plotData(k))
                
                %Update texfile
                fprintf(fileId,'\t%.8f',plotData(k));
            end
            fprintf(fileId,'\r\n');
            i = i + 1;
            %xlim([Time-WindowSize,Time])
            drawnow update
        end
    elseif(start_message == 65023)
        sampling = false;
        disp('Stop MATLAB')
    end
end

fclose(Arduino); %End communication with arduino
fclose(fileId);  %Close textfile