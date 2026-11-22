# Festival Grounds Roadmap

> Een tycoon- en managementgame geïnspireerd door klassieke simulatiespellen zoals RollerCoaster Tycoon 2, maar gericht op het bouwen en beheren van muziekfestivals.

---

# Fase 0 - Project Setup

## Doel
Een werkende technische basis creëren.

## Features

- [x] C++20 project
- [x] SDL2 integratie
- [x] CMake configuratie
- [x] Hoofdvenster
- [x] Game Loop
- [x] Event systeem
- [x] Asset Manager voor BMP-textures
- [x] Logging systeem

## Gereed wanneer

- [x] Applicatie start op
- [x] SDL2 venster verschijnt
- [x] FPS counter werkt
- [x] Programma netjes afsluit

---

# Fase 1 - Isometrische Wereld

## Doel

Een scrollbare isometrische kaart renderen.

## Features

- [x] Tile systeem (64x64)
- [x] Grass terrain met reproduceerbare kleur- en bloemvariatie
- [x] Camera beweging
- [x] Zoom systeem (scrollwiel en plus/min)
- [x] Reproduceerbare map generatie met seed
- [x] Isometric rendering met gebatchte terreintegels

## Gereed wanneer

- [x] 64x64 map zichtbaar
- [x] Camera kan bewegen
- [x] Zoom werkt
- [x] Tiles correct weergegeven

---

# Fase 2 - Bouwmodus

## Doel

Speler kan objecten plaatsen.

## Gebouwen

- [x] Pad
- [x] Gras-terrein als standaard ondergrond
- [x] Entree (start bij ingang en verplaatsbaar naar padtoegang)

## Features

- [x] Plaatsen
- [x] Verwijderen met bulldozer of rechtermuisknop
- [x] Geld aftrekken volgens gebouwdefinitie
- [x] Groene/rode bouwpreview
- [x] Gebouwen draaien met R
- [x] Gedeelde plaatsingsregels voor preview en bouwen

## Gereed wanneer

- [x] Pad plaatsen werkt
- [x] Gebouwen verwijderen werkt
- [x] Bouwkosten worden berekend
- [x] Ongeldige plaatsing wordt geblokkeerd

---

# Fase 3 - Bezoekers

## Doel

Eerste bezoekers simuleren.

## Features

- Visitor entity
- Visitor spawning
- Willekeurig rondlopen
- Basisanimatie

## Gereed wanneer

- [ ] Bezoekers verschijnen
- [ ] Bezoekers bewegen
- [ ] Entree wordt gebruikt

---

# Fase 4 - Pathfinding

## Doel

Bezoekers moeten slimme routes kiezen.

## Features

- Grid systeem
- A* pathfinding
- Route cache
- Dynamische herberekening

## Gereed wanneer

- [ ] Bezoekers volgen paden
- [ ] Doelen worden bereikt
- [ ] Vastlopen komt niet voor

---

# Fase 5 - Economie

## Doel

De eerste gameplay-loop maken.

## Gebouwen

- Food Stall

## Features

- Geldsysteem
- Inkomsten
- Uitgaven
- Dagoverzicht

## Gereed wanneer

- [ ] Bezoekers kopen eten
- [ ] Geld stijgt
- [ ] Gebouwen kosten geld

---

# Fase 6 - Behoeftesysteem

## Doel

Bezoekers gedrag realistischer maken.

## Needs

- Hunger
- Thirst
- Energy
- Happiness
- Toilet

## Gereed wanneer

- [ ] Behoeftes nemen af
- [ ] Bezoekers zoeken oplossingen
- [ ] Tevredenheid wordt berekend

---

# Fase 7 - Podia

## Doel

Bezoekers een reden geven om te blijven.

## Gebouwen

- Kleine Tent
- Open Podium

## Features

- Capaciteit
- Populariteit
- Podium aantrekkingskracht

## Gereed wanneer

- [ ] Bezoekers bezoeken podia
- [ ] Podia beïnvloeden tevredenheid

---

# Fase 8 - Dag/Nachtcyclus

## Doel

Een echte festivaldag simuleren.

## Tijden

- Ochtend
- Middag
- Avond
- Nacht

## Features

- Tijdsysteem
- Bezoekerspieken
- Verlichting

## Gereed wanneer

- [ ] Tijd verloopt
- [ ] Bezoekersaantallen veranderen

---

# Fase 9 - Personeel

## Doel

Management toevoegen.

## Personeel

- Beveiliging
- Schoonmaak
- Technici

## Gereed wanneer

- [ ] Personeel kan worden ingehuurd
- [ ] Taken worden uitgevoerd

---

# Fase 10 - Weersysteem

## Doel

Dynamische gebeurtenissen toevoegen.

## Weer

- Zon
- Regen
- Storm

## Effecten

- Modder
- Langzamere bezoekers
- Lagere tevredenheid

## Gereed wanneer

- [ ] Weer verandert dynamisch
- [ ] Weer beïnvloedt gameplay

---

# Fase 11 - Volledig Festival

## Doel

Eerste complete spelervaring.

## Nieuwe Features

- Artiesten
- Ticketprijzen
- Reputatie
- Meerdere podia
- Meerdaagse evenementen

## Gereed wanneer

- [ ] Volledige festivalcyclus speelbaar

---

# Fase 12 - Alpha Release

## Doel

Publiek speelbare versie.

## Features

- Save / Load
- Scenario's
- Statistieken
- Balancing
- Settings menu

## Gereed wanneer

- [ ] Volledig speelbaar
- [ ] Savegame werkt
- [ ] Basiscontent aanwezig

---

# Backlog (Post-Alpha)

## Grote Features

- Campings
- Parkeerplaatsen
- Elektriciteitsnetwerk
- Waternetwerk
- Onderzoekssysteem
- Workshop ondersteuning
- Mods
- Multiplayer
- Steam integratie
- Sandbox mode
- Scenario editor

---

# Technische Stack

- C++20
- SDL2
- CMake
- JSON Configs
- Eigen ECS Architectuur
- Eigen Isometric Renderer

---

# Eerste MVP Doel

## Playable Sandbox

- [ ] Isometrische wereld
- [ ] Paden plaatsen
- [ ] Food stalls bouwen
- [ ] Entree plaatsen
- [ ] Bezoekers spawnen
- [ ] A* pathfinding
- [ ] Geld verdienen

Wanneer alle punten afgerond zijn, is Festival Grounds officieel speelbaar.
