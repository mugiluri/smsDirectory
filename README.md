# smsDirectory :compass: - This project is developed from Arduino IDE with Eris V1 development board from Africas Talking with an STM32F103 chip based on 32-bit ARM Cortex M3
__pataNairobi__ :earth_africa: project

__#Overview:__
pataNairobi project focuses on sharing directories to people who are in need of them.
You might be stranded, you want to find someone or a place and you don't have any contacts or data/smartphone at the time. 
Maybe you are in a jungle with network issues. pataNairobi is the algorithm that takes care of that problem.

You will be able to do the following; 
* Chat :speech_balloon: with pataNairobi over text messages __*Send texts not exceeding 12 characters*__ 
* Inquire on weather conditions e.g __temperature__ and __humidity__ of Nairobi through text message in real time to prepare _for travellers_ 
* Send real time data to Africastalking cloud for analysis.

__#Note:__
Three Eris boards are used in this project; 
* For sending and receiving sms (_tophone board_) 
* Hardware resetting of the sms board (_resetboard_) 
* The IoT cloud updating board (_todashboard_)

__CONFIGURATIONS__
* _tophone board_ has a __personal sim card__, the number you send messages to
* _resetboard_ doesn't need to have a sim card
* _todashboard_ has an __Africastalking sim card__, to communicate with the IOT cloud

__CONNECTIONS__
For now, tophone and resetboard are connected via;
* __GND__ to __GND__ - _Common Ground_
* __PB1__ - _tophone_ connected to __PB5__ - _resetboard_
* __PB1__ - _resetboard_ connected to __RST__ _pin_ of the _tophone_ board 
* Connect the __DHT11 sensor__ to _tophone_ board via __PA7__ for the signal

__Currently the _todashboard_ works independently__
It updates sensor data to Africastalking cloud which;
* Can be viewed from a dashboard
* Can be downloaded and analysed

The objective is to make _todashboard_ read the serial data from _tophone_ board and send it to the IOT cloud.
Right now both _tophone board_ and _todashboard_ have sensors connected to them independently. 
With the serial exchange implementation, only the _todashboard_ will be connected to the sensors.
All the three boards will be connected and seamlessly working together. Feel free to do these improvements

__Log.txt:__ View this file in .gitignore to troubleshoot connectivity issues.



