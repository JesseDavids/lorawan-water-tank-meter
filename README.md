# LoraWan - Water Tank Meter 
by Jesse Davids


<h3>Introduction:</h3>

This water tank meter, calculates the distance from the sensor to the corresponding level of water inside the tank, 
sends the data off with lora protocols to The Things Network. Once it is in TTN we can retrieve the data into a 
database or application of your own creation.

<h3>Challenges:</h3>

First we were getting sloppy readings, until a blog post enlightened us on how ultrasonic waves work. With that
we bought some material, specifically "felt" and snugged it within the inner walls of the 50mm pipe, by 200mm in length. 
It absords the sounds you dont want to hear back from, which in turn can make the sensor very accurate if held at the 
right angle, remember, the sound wave needs to come back. The datasheet for the sensor shows it can reach up to
4.5meters. The tallest jojo tank is around 4.25meters.

