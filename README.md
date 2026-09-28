# InfinityOpsXenia
Xenia Canary plugin that loads GSC from game:\raw\. Built from [<egatobaS/InfinityOps_360>](<https://github.com/egatobaS/InfinityOps_360>).

## Description
Allows injecting custom .gsc on Call of Duty Black Ops 1 Xenia. Standard InfinityOps uses kernel/XAM functions which Xenia has not implemented. Xenia intercepts kernel functions and runs its own C++ replacements. Missing or stubbed ones are a common reason homebrew plugins error. 

## Implementation
A raw folder is to be created in the games directory "xenia_canary/games/Black Ops 1/raw". Your game should not be an iso image, you can use isoextract to get the raw files. You must use the most recent title update placed in the content folder. The raw folder is where you place your custom gsc files. The InfinityOps plugin should be placed in "xenia_canary/plugins/41560855/InfinityOps.xex". Within "xenia-canary.config.toml" you need to check allow_plugins = true. You need to create a plugins.toml where you have placed the InfinityOps.xex. Exmaple:
```c
title_name = "Call of Duty: Black Ops"
title_id = "41560855"

[[plugin]]
    name = "InfinityOps"
    file = "InfinityOps.xex"
    hash = "408AC9B9D7011930"
    desc = "GSC Injector for Black Ops 1"
    is_enabled = true
```
## "I have an issue"
thats cool I don't. 

# Credits
ImJtagModz, Sabotage, Blasts Mods and Stridder
