# GEM Guitar Tools

GEM Guitar Tools started out as an Atari ST application, now it is an HTML/JS application which you can find deployed on gitpages.

- https://eviltester.github.io/gem-guitar-tools/

When I started writing this 30+ years ago, there were not many Guitar apps.

Now there are many.

Here are some other similar tools:

- https://fretmap.app/
- https://www.fachords.com/guitar-scale/
- https://www.editor.guitarscientist.com/
- https://freetboard.online/
- https://fretastic.com/guitar
- https://muted.io/guitar-fretboard/
- https://fretlogic.studio/scale-finder
- https://www.gibson.app/tools/scale-finder
- https://guitar-fretboard-learning.web.app/

## History of gem-guitar-tools

Atari ST GEM Guitar Tools Reference

Back in 1993 I wrote my first commercial tool. This Guitar Reference, written in 'C' for the Atari ST.

It didn't sell particularly well, but I thought it was pretty good for the time.

I thought I had lost the source code, but stumbled across it. I can't remember what C compiler I used or anything like that.

Looking through the code I can see that I wrote my own very crude printer drivers. Ah, the good old days.

## Links

While getting this ready for original release I looked online to see if I could find any references

http://www.atarimax.com/freenet/freenet_material/6.16and32-BitComputersSupportArea/8.OnlineMagazines/showarticle.php?479

A review of the software I didn't know about

- archive.org - https://web.archive.org/web/20150701000000*/http://www.soundonsound.com/sos/1997_articles/may97/atarinotesmay97.html
    - http://www.soundonsound.com/sos/1997_articles/may97/atarinotesmay97.html

http://cd.textfiles.com/atarilibrary/atari_cd09/DOCUMENT/TEXTE/AEO_0407/AEO_0407.TXT

The original pages showcasing the Atari ST tool have been converted to markdown:

- [/htmlpages/readme.md](./htmlpages/readme.md)


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



