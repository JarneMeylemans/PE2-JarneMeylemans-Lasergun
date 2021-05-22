# Practice Enterprise 2: Lasergun

Jarne Meylemans 

(Finale code staat onder 'Project_Code/STM32F4/PE_LASERGUN_CODE_2apr')

## Korte Beschrijving

Het doel van dit project zal zijn te weten komen hoe een lasergun kan werken. Het geweer zal moeten kunnen een infrarood signaal sturen waarmee het bekend maakt wie het is (Elk geweer heeft zo zijn eigen "code". Elke Lasergun zal met een centraal punt kunnen communiceren om een score bij te houden van het houdige spelletje. 

Via een scherm kan de speler zien hoeveel kogels hij nog heeft en wat zijn score is in het spel. 

## Hardware

Om het lasergeweer te maken heb ik gekozen om met de STM32F410 microcontroller te werken, deze stuurt alles aan als volgt: 

![alt text](https://github.com/JarneMeylemans/PE2-JarneMeylemans-Lasergun/blob/main/Documenten/Foto's/blokschema.png?raw=true)

![alt text](https://github.com/JarneMeylemans/PE2-JarneMeylemans-Lasergun/blob/main/Documenten/Foto's/pcb.jpg?raw=true)

## Software 

Met de software heb ik geprobeerd om een soort State Machine te programmeren zodat er makkelijk kan veranderd worden van functie waar we inzitten

![alt text](https://github.com/JarneMeylemans/PE2-JarneMeylemans-Lasergun/blob/main/Documenten/Foto's/Mainc.png?raw=true)


## Behuizing

De behuizing is getekend in fusion 360 en is geprint met een 3D printer, de behuizing bestaat uit 11 verschillende stukken. 

![alt text](https://github.com/JarneMeylemans/PE2-JarneMeylemans-Lasergun/blob/main/Documenten/Foto's/behuizing.png?raw=true)
