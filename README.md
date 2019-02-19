# h-encore

*h-encore*, where *h* stands for hacks and homebrews, is the second public jailbreak for the *PS Vita™* which supports the newest firmwares 3.65, 3.67 and 3.68. It allows you to make kernel- and user-modifications, change the clock speed, install plugins, run homebrews and much more.

## Write-up

A technical explanation of the *h-encore* exploit chain is available [here](WRITE-UP.md).

## Changelog

#### Changelog h-encore 2.0:

- Added ability to auto-exit and bypass the bootstrap menu. You can force launching the bootstrap menu by holding R while launching *h-encore*).
- Added ability to personalize the savedata in order to get rid of the trophy warning.
- Added confirmation dialog for `Reset taiHEN config.txt` option.
- Updated default spoof version to 3.70.
- Updated kernel ROP chain to use fewer and better gadgets.

## Requirements

- Your device must be on firmware 3.65, 3.67 or 3.68. **Firmware 3.69 and higher are not supported**. If you're on a lower firmware, please decide carefully to what firmware you want to update, then search for a trustable guide on [/r/vitahacks](https://www.reddit.com/r/vitahacks/).
- If your device is a phat OLED model, you need a Memory Card in order to install. There's no need for a Memory Card on Slim/PS TV models, since they already provide an Internal Storage. Make sure you have got at least `270 MB` of free space.
- Your device must be linked to any PSN account (it doesn't need to be activated though). If it is not, then you must restore default settings in order to sign in.

## Installation

Note that the following guide is for advanced users and a bit more complicated than the previous hack that only required you to visit a website. If you don't understand the guide below or how to use these tools, you should not file an issue here, but rather seek help on [/r/vitahacks](https://www.reddit.com/r/vitahacks/comments/8v9vl7/biweekly_questions_thread_edition_23_hencore/) (check for duplicated questions first!) or use the [easy installer](https://github.com/soarqin/finalhe) (which isn't maintained by me).

1. Download [h-encore](https://github.com/TheOfficialFloW/h-encore/releases/download/v2.0/h-encore.zip) and extract it on your computer.

2. Download and install [qcma](https://codestation.github.io/qcma/), [psvimgtools](https://github.com/yifanlu/psvimgtools) and [pkg2zip](https://github.com/mmozeiko/pkg2zip) (check the releases section for the binaries).  
   If you don't know where to put psvimgtools and pkg2zip binaries, just put them in the `h-encore` folder.

3. Download the vulnerable DRM-free demo of [bitter smile](http://ares.dl.playstation.net/cdn/JP0741/PCSG90096_00/xGMrXOkORxWRyqzLMihZPqsXAbAXLzvAdJFqtPJLAZTgOcqJobxQAhLNbgiFydVlcmVOrpZKklOYxizQCRpiLfjeROuWivGXfwgkq.pkg) (yes, that's the user entry point).

4. Extract the demo using this command in terminal/cmd:
   ```
   pkg2zip -x PATH_OF_PKG
   ```

   This will output the files to `app/PCSG90096`.

5. Copy the contents of the output `app/PCSG90096` to the folder `h-encore/app/ux0_temp_game_PCSG90096_app_PCSG90096` (such that the files `eboot.bin` and `VITA_PATH.TXT` are within the same folder).

6. Copy the license file `app/PCSG90096/sce_sys/package/temp.bin` to the folder  
   `h-encore/license/ux0_temp_game_PCSG90096_license_app_PCSG90096` and rename the just pasted file `temp.bin` to ` 6488b73b912a753a492e2714e9b38bc7.rif`. Be careful with the file extension, it should not be `.rif.bin`. Again, this file should be in the same folder as `VITA_PATH.TXT`.

7. Start qcma and within the qcma settings set the option `Use this version for updates` to `FW 0.00 (Always up-to-date)` to spoof the System Software check.

8. Launch Content Manager on your PS Vita and connect it to your computer, where you then need to select `PC -> PS Vita System`, and after that you select `Applications`. If you see an error message about System Software, you should simply reboot your device to solve it (if this doesn't solve, then put your device into airplane mode and reboot). If this does still not work, then alternatively set DNS to `212.47.229.76` to block updates.
   This should create a folder at `PS Vita/APP/xxxxxxxxxxxxxxxx` on your computer (see qcma settings where this folder is), where the folder `xxxxxxxxxxxxxxxx` represents the AID (account ID that is 16 characters long) that you need to insert [here](http://cma.henkaku.xyz/). If the AID is valid, it will yield a key that you can now use to encrypt the demo.

9. Change directory to the `h-encore` folder in terminal/cmd and use the key to encrypt all folders using (make sure you don't confuse the key with the AID, the key is 64 characters long!):
   ```
   psvimg-create -n app -K YOUR_KEY app PCSG90096/app
   psvimg-create -n appmeta -K YOUR_KEY appmeta PCSG90096/appmeta
   psvimg-create -n license -K YOUR_KEY license PCSG90096/license
   psvimg-create -n savedata -K YOUR_KEY savedata PCSG90096/savedata
   ```

    The folder `h-encore/PCSG90096` should then contain `sce_sys` and all 4 folders from above, and within these folders you should find files called `X.psvimg` and `X.psvmd`, where `X` has the same name as the folder. Backup this folder, since if everything has been done correctly, you don't need to redo all the steps to install it onto another device with the same PSN account.

10. Copy the folder `h-encore/PCSG90096` to `PS Vita/APP/xxxxxxxxxxxxxxxx/PCSG90096` and then select `Refresh database` in qcma.

11. The *h-encore* bubble with a size of around `243 MB` should now appear in the Content Manager and that's what you finally need to transfer to your PS Vita. If the size does not match or you get the error `C2-12858-4`, then it's because you did not do it correctly! Please re-read the instructions more carefully then. If you get the error `You can only copy applications that your account is the owner of`, then it's because you have used an AID that is not of your account, go back to step 8.

12. Launch *h-encore* to exploit your device (if a message about trophies appears, simply click yes).
    The screen should first flash white, then purple, and finally open a menu called *h-encore bootstrap menu* where you can download [VitaShell](https://github.com/TheOfficialFloW/VitaShell) and install [HENkaku](https://github.com/henkaku).
    If it prompts the error `Cannot start this application. C0-11136-2`, then it's because you did not do step 6. correctly.

13. Enjoy. Note that you have to relaunch the exploit everytime you reboot or shutdown your device. Of course if you only put your device into standby mode, you don't need to relaunch.

## Updating to h-encore 2.0

You can update *h-encore* by following the installation guide above, or following these steps (*h-encore* must already be installed).

1. Download [h-encore's system.dat](https://github.com/TheOfficialFloW/h-encore/releases/download/v2.0/system.dat).
2. Enable `Unsafe Homebrews` under `HENkaku Settings` in the Settings application to grant VitaShell full permission.
3. Launch VitaShell and navigate to `ux0:user/00/savedata/`.
4. Press triangle on the folder `PCSG90096` and select `Open decrypted` (you should NOT see the folder `sce_pfs` within this folder when opened decrypted).
5. Copy the downloaded `system.dat` to `ux0:user/00/savedata/PCSG90096/system.dat`.
6. Launch *h-encore* while holding R and select `Install HENkaku` in the bootstrap menu.
7. Done.

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

- "Can I install enso on 3.67 or 3.68?" - Not on these firmwares, but you can downgrade to firmware 3.65 using [modoru](https://github.com/TheOfficialFloW/modoru) and then install enso.
- "Can I install enso on 3.65?" - Yes, you can use *h-encore* to hack your device and then install the permanent hack using [this](https://github.com/TheOfficialFloW/enso/releases).

### Compatibility

- "Are Adrenaline/NoNpDrm/Download Enabler supported on 3.65/3.67/3.68?" - Yes, check them in my repositories.
- "Can I use SD2VITA using this hack?" - Yes, I have made a pull request on [gamecard-microsd](https://github.com/xyzz/gamecard-microsd) that fixed the freeze when using it without enso. If you're using an other plugin and it freezes on exitting *h-encore bootstrap menu*, then there's the trick where you can simply press the PS Button and return back to finish the boot process.
- "Can I use psvsd using this hack?" - Yes, people confirmed that it is working finely.
- "Does this work, does that work? Is this compatible, is that compatible?" - I don't know, and it is not my task to update these tools for you, so don't dare and file an issue here.

### General

- "Can I switch the PSN account after having *h-encore* installed?" - Yes, since the demo is DRM-free it does not depend on your account.
- "Are there any risks involved in using *h-encore*?" - No, since it does not modify the OS, but only insert temporary patches into the system.
- "Can I install it without USB connection?" - You can also connect your PS Vita with your computer using Wi-Fi (there's an option in the Content Manager).

## Donation

If you like my work and want to support future projects, you can make a donation:

- via bitcoin `361jRJtjppd2iyaAhBGjf9GUCWnunxtZ49`
- via [paypal](https://www.paypal.me/flowsupport/20)
- via [patreon](https://www.patreon.com/TheOfficialFloW)

Thank you!

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
