# &ensp;Reaction
This challenge is made to learn how to process information from sensors correctly, get aqcuanted with basic data filtering algorithms (median filter, moving average filter, low pass filter etc).

## &ensp; &ensp;  <b> How to run this project? </b>

<b> &ensp; Hardware </b>
 - [Board LAUNCHXL-CC1352R1](https://www.ti.com/tool/LAUNCHXL-CC1352R1#description)
  - [Board BOOSTXL-SENSORS](https://www.ti.com/tool/BOOSTXL-SENSORS)
 - Micro USB cable

<b> &ensp; Software </b>
 - [CodeComposerStudio](https://www.ti.com/tool/download/CCSTUDIO) (CCS)
- SimpleLink SDK for your given LaunchPad (CCS may recomend you to install one)
 - [UniFlash](https://www.ti.com/tool/download/UNIFLASH)

 ### &ensp; &ensp; Running this project
 - git clone
 - open project with CCS (NB! One folder per time i.e t00 only or t04 only)
 - select `[project_name]` in your project explorer
 - press `hammer` icon to build this project
 - there are various ways to flash the board with built project:
    - select `[project_name]` in your project explorer and ...
   - ... press `curly brackets in a folder` icon
   </br>OR
   - ... press Run -> Load -> `[project_name]`
![screenshot](https://user-images.githubusercontent.com/54025456/109845882-43b68980-7c56-11eb-97dd-72f7ce694c9f.png)

   #### OR
   - Open <b>UniFlash</b> and follow [this video guide](https://www.youtube.com/watch?v=V3-xcRmu5S0&t=51s) for device autodetect, and [this guide](http://software-dl.ti.com/ccs/esd/uniflash/docs/v5_0/quick_start_guide/uniflash_quick_start_guide.html) to flash the device.

## &ensp;  What to `.gitignore`?
Which CCS project files should be checkd in, and which should be `.gitignored`? [Read here](https://software-dl.ti.com/ccs/esd/documents/sdto_ccs_source-control.html)
