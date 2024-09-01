My Drink Machine Project from 2020
__________________________________

Story:

After high school, I was eager to test my skills and dive into the world of 3D printing and Arduino programming. 
My mission was simple: build a cheap drink machine that could mix drinks. Mission accomplished? Not exactly. But honestly, I wouldn't change a thing about this beautiful disaster.
In my quest for “cheap,” I decided to use only two motors, thinking they would be the priciest part of the build. Spoiler alert: this decision made the whole project more complex and expensive. 
My brilliant plan was to have one stepper motor select the liquor and one DC motor pump it into the glass. Sounds easy, right? 
It would have been, had I not ignored the existence of gravity and the simplicity of valves. But where’s the fun in that?
Bacteria and cleanliness quickly became top priorities. Turns out, tiny 3D-printed crevices and printed plastic don’t mix well with food and beverages. So, I opted for silicone tubes as the liquor pathways.
After a ton of trial and error, I can proudly say I made 3D-printed peristaltic pumps that ensure no liquor ever touches anything but the tube.
The 12V DC motor I used was originally designed for RC race cars, boasting an impressive 60,000 RPM. Fast? Absolutely. Suitable for driving peristaltic pumps? Not even close. But did I buy a new motor? 
Of course not! Instead, I learned how to design and 3D-print gears to convert high RPM and low torque into low RPM and high torque. The result? A machine that sounds like a jet engine every time it pours a drink. 
Who needs peace and quiet when you have a masterpiece like this?
The power supply and motor drivers were swapped out multiple times for bigger, stronger versions because the motors’s start current was off the charts. 
In the end, after countless added features (see the list below) and more learning experiences than I can count, I ended up with a machine that could actually make a drink. 
Not a perfect drink, mind you. Liquors have different buoyancies, making the liquid handling tricky. Some liquids would just fall out of the tube, while others stubbornly stuck to it. 
Even with a whole calibration system built in, the drinks sometimes came out stronger or weaker than intended.
But hey, who doesn’t love a little surprise in their cocktail? Cheers to that!

----------------------------------
Features:

- Controlled by an ARDUINO MEGA.
- Operated via a TFT LCD DISPLAY, waterproof colored LED buttons, and a rotary encoder.
- Internal power converters ensure everything is powered by the 12V power supply.
- COOL LED LIGHTING illuminates the interior pumps, visible through the plexiglass. These are controlled by a separate ARDUINO NANO and LED driver, managing 20+ built-in presets that can be selected as desired.
- Hidden push-button to toggle between setup and running modes. In setup mode, an extra menu allows the user to add ingreedients and calibrate pumps.
- SD card for storing recipes and ingredients. Easily removable for adding more at any time. The custom-made C# program "DrinkManager" makes it simple to add new recipes to the SD card.
- Fully 3D-printable on any printer, with all parts assembled using bolts and nuts.

- Supports up to 8 tubes and ingredients at a time. Ingredients can be selected from the menu and assigned to the pumps. Simply place the tube into the drink.
- Calibration menu to fine-tune the pumps based on the liquids’ buoyancies. Weight is set automatically during calibration: start-stop when 1 deciliter is poured.
- Adjustable height for the tube’s output, accommodating any size glass or cup without splashing.
- Tray underneath to catch spilled liquid.
- After assigning ingredients to the pumps, the machine will automatically scan all recipes on the SD card to see which drinks can be mixed with the available ingredients.
- Ingredients in recipes are marked as either soft (soda, etc.) or hard (alcohol). This allows users to adjust the strength of their drink, with the machine automatically scaling the soft/hard ratio to achieve the desired strength.
- Cupholder included.
- Lids secured by magnets and custom-made knobs on the backplate ensure smooth installation. Pumps can be easily disassembled to insert or remove the tubes for cleaning and setup.
- Progress bar while the machine is mixing the drink.
