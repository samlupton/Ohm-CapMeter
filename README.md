# Capacitance and Resistance Meter with LCP1769 Micro Controller

This repository comprises of source code and schematics for accurately measuring resistance and capacitance using an LCP1769 microcontroller. The code has been written entirely in C++, ensuring reliability and consistency in the measurements.

The capacitance meter code has been meticulously calibrated to measure capacitance values spanning from 0.001μF to 100μF. Similarly, the resistance meter has been fine-tuned to accurately measure resistance values within the range of 100Ω to 1MΩ.

For those wanting to replicate this project, you will use the MCUXpresso IDE. Set the project type as C/C++ (semi-hosted).

## ChatGPT Insight

### ChatGPT provided a wealth of valuable insights on the topic of measuring electrical components. Here are some of the chatboxes responses.

* For the capacitor range you specified (0.001 μF to 10 μF), a good resistor value to use would be around 1 kΩ to 100 kΩ. 
This will give you a time constant between 10 μs and 10 ms, which is suitable for this range of capacitance values.
If you use a resistor value of 10 kΩ, for example, the time constant would be: τ = RC = 10 kΩ x 0.001 μF = 10 μs
This would allow you to measure capacitors as small as 0.001 μF with a resolution of 10 μs.
On the other hand, if you use a resistor value of 100 kΩ, the time constant would be: τ = RC = 100 kΩ x 10 μF = 1 s
This would allow you to measure capacitors as large as 10 μF with a resolution of 1 second. Keep in mind that the accuracy 
of your capacitance meter will depend on the accuracy of the components used and the circuit design, so you may need to calibrate 
your meter for more precise measurements. 

* Using multiple circuits with different resistor values can be a good approach to increase the measurement range and resolution of 
your capacitance meter. By using different resistor values, you can create multiple RC time constants and cover a wider range of 
capacitance values. For example, you could use three circuits with resistors of 1 kΩ, 10 kΩ, and 100 kΩ, respectively. This would 
give you time constants of 1 μs, 10 μs, and 100 μs, covering a capacitance range from 0.001 μF to 10 μF with a resolution of 1 μs.
To implement multiple circuits, you can use a multiplexer or switch to select the appropriate resistor for each measurement. You can 
then measure the charging time for each circuit and calculate the capacitance value using the appropriate time constant.
Keep in mind that using multiple circuits can increase the complexity and cost of your capacitance meter, and you may need to consider
factors such as switching speed and signal interference. However, it can be a useful approach to improve the accuracy and versatility of 
your measurements.

While I recommend the use of ChatGPT as a useful resource for accessing detailed knowledge quickly, it is important to note that as with any information source, the accuracy and reliability of the information provided by ChatGPT cannot always be guaranteed. While the model is trained on a vast amount of data, it may generate false or inaccurate information in some cases. Therefore, it is always a good idea to cross-check information obtained from ChatGPT with other reputable sources and exercise caution when relying solely on its responses.


## Note
* It is important to note that the accuracy of the measurements obtained may be affected by various factors such as the actual values of the load resistors and system noise. To mitigate these factors, it is recommended to measure the load resistors to be used and input their values into the code accordingly before running it. Furthermore, to obtain even more accurate readings, an array has been implemented to calculate the average value of the components and account for any noise present in the system. Depending on the level of noise present in your system, it may be necessary to increase the number of elements in the array to obtain more precise results.

## Schematics and Drawings
To ensure accurate measurements, the voltage dividers have been programmed to allow only one load resistor to be connected to Vcc at any given time. All other resistors are set to float based on analog-to-digital values read at pin 15.

<img src="thumbnail_image.png" alt="My Image" width="400" height="300">

To accurately measure the capacitance values, the analog-to-digital values are read from pin 15. Once the voltage divider value is read at zero volts, a timer is initiated. The capacitor is then charged until it reaches 2/3 of Vcc, while the timer captures the time taken. This time is then set as the RC time constant, τ, and simple mathematical calculations are performed to solve for the capacitor value.

<img src="thumbnail_image (1).png" alt="My Image" width="400" height="300">

## Resources 

* The most valuable resource was my partner, Nhi Luong, an accomplished electrical engineer from Oklahoma. Should you need to get in touch with her, her email address is nhi.luong13@gmail.com. 
* This [link](https://www.eecs.tufts.edu/~dsculley/tutorial/rc/rc3.html#:~:text=The%20RC%20time%20constant%20is,the%20resistance%20R%2C%20in%20Ohms) is a great resource for understanding how one can measure a capacitor using the RC time constant.
