# GEM Guitar Tools

GEM Guitar Tools started out as an Atari ST applcation

## History of gem-guitar-tools

Atari ST GEM Guitar Tools Reference

Back in 1993 I wrote my first commercial tool. This Guitar Reference, written in 'C' for the Atari ST.

It didn't sell particularly well, but I thought it was pretty good for the time.

I thought I had lost the source code, but stumbled across it. I can't remember what C compiler I used or anything like that.

Looking through the code I can see that I wrote my own very crude printer drivers. Ah, the good old days.

## Current Links

While getting this ready for release I looked online to see if I could find any references

http://www.atarimax.com/freenet/freenet_material/6.16and32-BitComputersSupportArea/8.OnlineMagazines/showarticle.php?479

A review of the software I didn't know about

- archive.org - https://web.archive.org/web/20150701000000*/http://www.soundonsound.com/sos/1997_articles/may97/atarinotesmay97.html
    - http://www.soundonsound.com/sos/1997_articles/may97/atarinotesmay97.html

http://cd.textfiles.com/atarilibrary/atari_cd09/DOCUMENT/TEXTE/AEO_0407/AEO_0407.TXT

apparently I had a web page for it at `www.wmin.ac.uk/~richara/guitaref.htm` but I don't remember what was there.

It might even be the files in htmlpages:

- [/htmlpages/readme.md](./htmlpages/readme.md)

## GitHub Pages

Live web app (after deployment from `main`):

- [https://<your-github-username>.github.io/gem-guitar-tools/](https://<your-github-username>.github.io/gem-guitar-tools/)

Licence
--------
Ignore any licence text you find inside the code or archive. I hereby release this thing into the public domain 12/12/2012 - do what thou wilt with it.

## Playwright Execution

From `webapp`, run end-to-end tests with a visible browser:

```bash
npm run test:e2e -- --headed
```

Useful variants:

```bash
# Run with one worker (easier to watch)
npm run test:e2e -- --headed --workers=1

# Step-through debug mode
npm run test:e2e -- --debug

# Open Playwright UI mode
npx playwright test --ui
```



