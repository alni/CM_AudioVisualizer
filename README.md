# CM_AudioVisualizer

Audio Visualizer for Cooler Master peripherals

## Usage 

Using default settings: (threshold at 0.01/1% ~~0.30~~)

```
CM_AudioVisualizer.exe
```


Setting the threshold to 0.40

```
CM_AudioVisualizer.exe threshold 0.40
```


Allowing full range output (setting the threshold to 1%)

```
CM_AudioVisualizer.exe threshold 0.01
```


Allowing full range output _and_ amplify weak/quiet audio (setting the threshold to 1% and setting the multiplier)

```
CM_AudioVisualizer.exe threshold 0.01 multiplier 5.0
```

Using the default threshold and weaken loud/noisy audio (only setting the multiplier)

```
CM_AudioVisualizer.exe multiplier 0.20
```

### Parameters

* `threshold <num>` - Setting the activation threshold to a specific number
  * **Range** - `0.01 - 0.99`
  * **Default** - `0.01` ~~`0.30`~~
* `multiplier <num>` - Multiplier for the audio level (useful for example with low audio level sounds to amplify/increase their brightness)
  * **Range** - `0.01 - 10.0` (falls back to the default value if outside of this range)
  * **Default** - `1.0`
* `device <device>` - The device to use (the value is the same as the `DEVICE_INDEX` enum variables, but without the `DEV_` prefix)
  * **Supported values**
    * `CK372` (keyboard)
    * `CK530` (keyboard)
    * `CK550_552` (CK550 + CK552 keyboards)
    * `CK551` (keyboard)
    * `MK750` (keyboard)
    * `MK850` (keyboard)
    * `MKeys_L` (MasterKeys Pro L keyboard)
    * `MKeys_L_White` (MasterKeys Pro L White keyboard)
    * `MKeys_M` (MasterKeys Pro M keyboard)
    * `MKeys_M_White` (MasterKeys Pro M White keyboard)
    * `MKeys_S` (MasterKeys Pro S keyboard)
    * `MKeys_S_White` (MasterKeys Pro S White keyboard)
    * `MM520` (mouse)
    * `MM530` (mouse)
    * `MM830` (mouse)
    * `MMouse_L` (MasterMouse Pro L)
    * `MMouse_S` (MasterMouse S)

