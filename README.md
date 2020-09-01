# smsDirectory :compass: - This project is developed from Arduino IDE with Eris V1 development board from Africastalking
__pataNairobi__ :earth_africa: project

__#Overview:__
pataNairobi project focuses on sharing directories to people who are in need of them.
You might be stranded, you want to find someone or a place and you don't have any contacts or data/smartphone at the time. 
Maybe you are in a jungle with network issues. pataNairobi is the algorithm that takes care of that problem.

You will be able to do the following; 
* Chat :speech_balloon: with patanairobi over text messages __*Send texts not exceeding 12 characters*__ 
* Inquire on weather conditions e.g temperature and humidity of Nairobi through text message in real time to prepare _for travellers_ 
* Send ral time data to Africastalking cloud for analysis.

__#Note:__
Three Eris boards are used in this project; 
* For sending and receiving sms (_tophone board_) 
* Hardware resetting of the sms board (_resetboard_) 
* The IoT cloud updating board (_todashboard_)

__CONFIGURATIONS__
* _tophone board_ has a personal sim card, the number you send messages to
* _resetboard_ doesn't need to have a sim card
* _todashboard_ has an Africastalking sim card, to communicate with the IOT cloud

__CONNECTIONS__
For now, tophone and resetboard are connected via;
* GND to GND - _Common Ground_
* __PB1__-_tophone_ connected to __PB5__ - _resetboard_
* __PB1__ - _resetboard_ connected to __RST__ _pin_ of the _tophone_ board 
* Connect the __DHT11 sensor__ to _tophone_ board via __PA7__ for the signal

___Currently the _todashboard_ works independently__
It updates sensor data to Africastalking cloud which;
* Can be viewed from a dashboard
* Can be downloaded and analysed
The objective is to make it read the serial data from _tophone_ board and send to cloud
Feel free to do those improvements



