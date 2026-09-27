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

## Update

```bash
omarchy theme update
```

That pulls every theme you installed from git. When Papaya is the active theme, the hook refreshes the desktop from this repo. New wallpapers stay on disk. The current wallpaper is left where it is.

`colors.toml` is the palette. Omarchy Quattro builds the terminal, browser, editor, and shell colors from it. Do not commit generated app configs into this repo.
