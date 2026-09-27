# Papaya

McLaren F1 theme for [Omarchy](https://omarchy.org/). Cool carbon black, with papaya `#ff8000` as the accent for the shell, window borders, terminals, Chrome, and the other apps Omarchy themes from `colors.toml`.

## Install

```bash
omarchy theme install https://github.com/maplepreneur/Papaya.git
```

Omarchy names the theme `papaya` from this repository.

Turn on the update hook once, in the installed copy:

```bash
git -C ~/.config/omarchy/themes/papaya config core.hooksPath .githooks
```

## Bar logo and screensaver

The speedmark is in `logo/`, and the screensaver art is `screensaver.txt`. Omarchy's installer applies colors and wallpapers. It does not run a script from the theme, so the logo and screensaver need one command on a new machine:

```bash
omarchy theme install https://github.com/maplepreneur/Papaya.git
~/.config/omarchy/themes/papaya/bin/install-logo
```

That replaces the Omarchy mark on the left of the bar and installs the screensaver. Left click still opens the menu. Right click still opens a terminal.

The command also registers a theme hook. After that, `omarchy theme install` and `omarchy theme set papaya` apply the logo and screensaver themselves, because both finish by setting the theme.

Selected text in Chrome and Chromium is papaya `#ff8000`. Restart the browser after installing or updating the theme so it loads that highlight.

## Update

```bash
omarchy theme update
```

That pulls every theme you installed from git. When Papaya is the active theme, the hook refreshes the desktop from this repo. New wallpapers stay on disk. The current wallpaper is left where it is.

`colors.toml` is the palette. Omarchy Quattro builds the terminal, browser, editor, and shell colors from it. Do not commit generated app configs into this repo.
