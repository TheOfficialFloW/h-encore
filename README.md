# h-encore

*h-encore*, where *h* stands for hacks and homebrews, is the second public jailbreak for the *PS Vita™* which supports the newest firmwares 3.65, 3.67 and 3.68. It allows you to make kernel- and user-modifications, change the clock speed, install plugins, run homebrews and much more.

## Requirements

- Your device must be on firmware 3.65, 3.67 or 3.68. Any other is not supported. If you're on a lower firmware, please decide carefully to what firmware you want to update, then search for a trustable guide on [/r/vitahacks](https://www.reddit.com/r/vitahacks/) (if you know how and want to contribute, you can edit this readme and make a pull request, such that fellow readers have got an easier life).
  Remember that on firmware 3.65 you have got the possibility to install [enso](https://github.com/TheOfficialFloW/enso/releases), the permanent hack, whereas on 3.67 and 3.68 you don't.
- If your device is a phat OLED model, you need a Memory Card in order to install. There's no need for a Memory Card on Slim/PS TV models, since they already provide an Internal Storage. Make sure you have got at least `270 MB` of free space.
- Your device must be linked to any PSN account (it doesn't need to be activated though).

## Installation

Note that the following guide is for advanced users and a bit more complicated than the previous hack that only required you to visit a website. If you don't understand the guide below or how to use these tools, you should neither file an issue here nor annoy me on twitter, but rather seek help on [/r/vitahacks](https://www.reddit.com/r/vitahacks/) (check for duplicated questions first!) or wait for an easy installer by somebody else.

1. Download and install [qcma](https://codestation.github.io/qcma/), [psvimgtools](https://github.com/yifanlu/psvimgtools) and [pkg2zip](https://github.com/mmozeiko/pkg2zip) (check the releases section for the binaries).
2. Download the vulnerable DRM-free demo of [bitter smile](http://ares.dl.playstation.net/cdn/JP0741/PCSG90096_00/xGMrXOkORxWRyqzLMihZPqsXAbAXLzvAdJFqtPJLAZTgOcqJobxQAhLNbgiFydVlcmVOrpZKklOYxizQCRpiLfjeROuWivGXfwgkq.pkg) (yes, that's the user entry point).
3. Download [h-encore](https://github.com/TheOfficialFloW/h-encore/releases/download/v1.0/h-encore.zip) and extract it on your computer.
4. Extract the demo using this command in terminal:
   ```
   pkg2zip -x PATH_OF_PKG
   ```
   
   This will output the files to `app/PCSG90096`.
5. Copy the contents of the output `app/PCSG90096` to the folder `h-encore/app/ux0_temp_game_PCSG90096_app_PCSG90096` (such that the files `eboot.bin` and `VITA_PATH.TXT` are within the same folder).
6. Copy the license file `app/PCSG90096/sce_sys/package/temp.bin` to the folder  
   `h-encore/license/ux0_temp_game_PCSG90096_license_app_PCSG90096` and rename the just pasted file `temp.bin` to ` 6488b73b912a753a492e2714e9b38bc7.rif`. Again, this file should be in the same folder as `VITA_PATH.TXT`.
7. Start qcma and within the qcma settings set the option `Use this version for updates` to `FW 0.00 (Always up-to-date)`.
8. Launch Content Manager on your PS Vita and connect it to your computer, where you then need to select `PC -> PS Vita System`, and after that you select `Applications`. If you see an error message about System Software, you should simply reboot your device to solve it.
   This should create a folder at `PS Vita/APP/xxxxxxxxxxxxxxxx` on your computer (see qcma settings where this folder is), where the folder `xxxxxxxxxxxxxxxx` represents the AID (account ID and 16 characters long) that you need to insert [here](http://cma.henkaku.xyz/). If the AID is valid, it will yield a key that you can now use to encrypt the demo.
9. Change directory to the `h-encore` folder in terminal and use the key to encrypt all folders using (make sure you don't confuse the key with the AID, the key is 64 characters long!):
   ```
   psvimg-create -n app -K YOUR_KEY app PCSG90096/app
   psvimg-create -n appmeta -K YOUR_KEY appmeta PCSG90096/appmeta
   psvimg-create -n license -K YOUR_KEY license PCSG90096/license
   psvimg-create -n savedata -K YOUR_KEY savedata PCSG90096/savedata
   ```

    The folder `h-encore/PCSG90096` should then contain `sce_sys` and all 4 folders from above, and within these folders you should find files called `X.psvimg` and `X.psvmd`, where `X` has the same name as the folder. Backup this folder, since if everything has been done correctly, you don't need to redo all the steps to install it onto another device with the same PSN account.
10. Copy the folder `h-encore/PCSG90096` to `PS Vita/APP/xxxxxxxxxxxxxxxx/PCSG90096` and refresh the database under qcma settings.
11. The *h-encore* bubble with a size of around `243 MB` should now appear in the Content Manager and that's what you finally need to transfer to your PS Vita.
12. Launch *h-encore* to exploit your device (if a message about trophies appears, simply click yes).
    The screen should first flash white, then purple, and finally open a menu called *h-encore bootstrap menu* where you can download [VitaShell](https://github.com/TheOfficialFloW/VitaShell) and install [HENkaku](https://github.com/henkaku).
13. Enjoy. Note that you have to relaunch the exploit everytime you reboot or shutdown your device. Of course if you only put your device into standby mode, you don't need to relaunch.

## Getting rid of the trophy warning

Since the savedata that I provided is not linked to your account id, launching *h-encore* would eventually show a warning message about trophies. To get rid of that you must do this on your PS Vita:

1. Enable `Unsafe Homebrews` under `HENkaku Settings` in the Settings application to grant VitaShell full permission.
2. Launch VitaShell and navigate to `ux0:user/00/savedata/`.
3. Press triangle on the folder `PCSG90096` and select `Open decrypted`.
4. Copy `system.dat` out of the folder and remove the folder `ux0:user/savedata/00/PCSG90096`.
5. Close VitaShell and launch *h-encore* (this will now not trigger the exploit anymore, since we've removed the savedata).
6. Close the game after you see the first screen and return back to VitaShell.
7. A fresh folder of `PCSG90096` at `ux0:user/savedata/00` should have been created.
8. Copy `system.dat` back to `ux0:user/savedata/00/PCSG90096/system.dat` where you need to open the `PCSG90096` using `Open decrypted`.
9. Now if you launch *h-encore* you should not see the trophy warning anymore and exploiting your device is therefore even faster.

## FAQ

### Exploit

- "When I launch *h-encore*, it stays at a white screen." - Due to the nature of the kernel exploit, this can sometimes happen. If it stays white for more than 5 seconds, you can simply close the application which will result in a crash and your device will be rebooted or shutdown after 10 seconds. If it doesn't, hold the power button down for over 30 seconds to force a shutdown. Then try the exploit again. The success rate of the kernel exploit should be at 80%. If I find time I will eventually try to improve the success rate.
- "When I launch *h-encore*, it flashes white quickly and then crashes." - Again, this is due to how the kernel exploit works.
- "I get a C2-12828-1 error when launching *h-encore*" - This does sometimes (but very rarely) happen. Just retry the exploit.
- "When I launch *h-encore*, it launches the bitter smile demo instead." - Your savedata is either corrupted or not installed correctly, please follow the installation guide above to reinstall it.
- "I have installed a bad plugin and launching *h-encore* doesn't work anymore, what should I do?" - You can either reset taiHEN config.txt or skip plugins loading by holding the L trigger while exiting the *h-encore bootstrap menu*.

### HENkaku Settings

- "I don't see all folders in VitaShell." - Launch the Settings application and select `HENkaku Settings`, then select `Enable unsafe homebrews`. This will grant you full permission in VitaShell.
- "I can't find the HENkaku Settings." - Launch the exploit and reset taiHEN config.txt and reinstall HENkaku.

### enso/permanent hack

- "Can I install enso on 3.67 or 3.68?" - Not yet, since molecule's bootloader exploit has been patched there.
- "Can I install enso on 3.65?" - Yes, you can use *h-encore* to hack your device and then install the permanent hack using [this](https://github.com/TheOfficialFloW/enso/releases).

### Compatibility

- "Are Adrenaline/NoNpDrm/Download Enabler supported on 3.65/3.67/3.68?" - Yes, check them in my repositories.
- "Can I use SD2VITA using this hack?" - Yes, I have made a pull request on [gamecard-microsd](https://github.com/xyzz/gamecard-microsd) that fixed the freeze when using it without enso. If you're using an other plugin and it freezes on exitting *h-encore bootstrap menu*, then there's the trick where you can simply press the PS Button and return back to finish the boot process.
- "Can I use psvsd using this hack?" - It should, but I'm not sure, as I haven't tested it. If it doesn't, then the driver must be updated, but that's not my task.
- "Does this work, does that work? Is this compatible, is that compatible?" - I don't know, and it is not my task to update these tools for you, so don't dare and file an issue here.

### General

- "Can I switch the PSN account after having *h-encore* installed?" - Yes, since the demo is DRM-free it does not depend on your account.
- "Are there any risks involved in using *h-encore*?" - No, since it does not modify the OS, but only insert temporary patches into the system.
- "Can I install it without USB connection?" - You can also connect your PS Vita with your computer using Wi-Fi (there's an option in the Content Manager).

## Donation

If you like my work and want to support future projects, you can make a donation:

- via bitcoin `361jRJtjppd2iyaAhBGjf9GUCWnunxtZ49`
- via [paypal](https://www.paypal.me/flowsupport/20)

You certainly don't have to donate to me, but if you do via paypal, please consider doing it next week or even next month, since a (small) transactions flood will be quickly noticed, especially if they include some messages, you know which 😉, and my account will be blocked as a result. I really don't want to sound greedy or rude, I highly appreciate your kindness, but because of the same reason, small donations (<5$) are not desirable. Oh and if you're blogging about this release, also don't include the paypal link in your post, but simply link this github repository.

Thank you!

## Source code

The source code and the kernel exploit will be made public after Sony has fixed the bugs used in *h-encore*.

## Credits

- Thanks to Freakler for finding the crash in the demo and designing the *h-encore* icon.
- Thanks to molecule for their initial work on the PS Vita.
- Thanks to xyz for giving me some tips on choosing an exploit target.
- Thanks to Davee and Proxima for http://cma.henkaku.xyz/.
- Thanks to yifanlu for psvimgtools.
- Thanks to codestation for qcma.
- Thanks to mmozeiko for pkg2vita.
- Thanks to the PS Vita hacking community.
- Thanks to Sony for this awesome device.
