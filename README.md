# Festival Grounds

Festival Grounds is een speelbare C++20- en SDL2-prototypegame over het bouwen en beheren van een muziekfestival. De kaart is isometrisch en 64×64 tegels groot. De huidige versie bevat bouwmodus, bezoekers, A*-routeplanning en een eenvoudige festival-economie.

De renderer gebruikt een canvas van 640×400 pixels en schaalt dat scherp op. Gebouwen en bezoekers worden met SDL2-vormen getekend; externe spritebestanden zijn niet nodig.

## Benodigdheden

- CMake 3.20 of nieuwer
- Een C++20-compiler
- Internettoegang tijdens de eerste configuratie als SDL2 niet al als CMake-package is geïnstalleerd

Als SDL2 niet wordt gevonden, downloadt CMake de officiële SDL2 2.32.8-broncode en bouwt die statisch mee.

## Bouwen en starten

### Windows

Installeer CMake en Visual Studio Build Tools met de workload **Desktop development with C++**. Dubbelklik daarna op `start.bat`; die configureert, bouwt en start de game. Vanuit PowerShell kun je `setup.ps1` gebruiken om alleen te configureren, of `setup.ps1 -Build` om te configureren en bouwen zonder de game te starten:

```powershell
.\setup.ps1
.\setup.ps1 -Build
```

Sluit Festival Grounds voordat je opnieuw bouwt. Windows vergrendelt de `.exe` zolang de game draait; de setup geeft een melding als het proces nog actief is.

### Handmatig bouwen

```sh
cmake -S . -B build
cmake --build build --config Release
```

Start daarna `build/Release/festival_grounds.exe` met Visual Studio op Windows, `build/festival_grounds.exe` met een single-configuration generator, of `./build/festival_grounds` op Linux.

## Spelen

Je begint op een grasveld met een ingang en een kort pad. Verleng het pad en plaats voorzieningen ernaast. De ingang kun je verplaatsen naar een lege tegel naast een pad. De groene of rode tegelpreview laat zien of de gekozen plek geldig is.

| Invoer | Actie |
| --- | --- |
| WASD of pijltjestoetsen | Camera verplaatsen |
| Home | Kaart centreren |
| Muiswiel, `+` of `-` | In- en uitzoomen |
| `1` | Pad selecteren ($5 per tegel) |
| `2` | Eetkraam selecteren ($120) |
| `3` | Toilet selecteren ($80) |
| `4` | Klein podium selecteren ($450) |
| `5` | Ingang verplaatsen (gratis) |
| Linkermuisknop | Geselecteerd object plaatsen of slopen |
| `B` | Bulldozer selecteren |
| Rechtermuisknop | Tegel slopen |
| `R` | Geselecteerd gebouw draaien |
| Spatie | Simulatie pauzeren of hervatten |
| `N` | Meteen naar de volgende festivaldag |
| Escape | Spel afsluiten |

Je begint met $2.500. Voor inkomsten bouw je een eetkraam naast een aangesloten pad; die kost $120. Bezoekers komen binnen via de ingang, lopen over aangesloten paden, bezoeken podia, kopen eten, gebruiken toiletten en verlaten het terrein. Hun behoeften en bestemmingen verschillen per bezoeker. Een voedselverkoop levert $18 op; aan het einde van de dag wordt $2 per verkoop aan voorraadkosten afgetrokken. De HUD toont saldo, bezoekersaantal, dagfase en zoomniveau.

## Technische opzet

- `src/main.cpp` beheert de SDL2-applicatie, invoer, vaste simulatiestappen en rendering.
- `src/core/` bevat gedeelde types, een eventbus, logging en een BMP-texturemanager.
- `src/world/` beheert de 64×64-tegelkaart, gebouwplaatsing en de navigatieversie.
- `src/pathfinding/` bevat A* en een begrensde routecache. Padwijzigingen maken de cache ongeldig; bezoekers plannen actieve routes dan opnieuw.
- `src/simulation/` beheert bezoekers, behoeften, aankomst, gedrag en beweging.
- `src/economy/` beheert bouwkosten, voedselinkomsten en dagtotalen.
- `src/rendering/` tekent de isometrische kaart, bezoekers, gebouwen en HUD met SDL2.
- `src/ui/` verwerkt toetsenbord- en muisinvoer.
- `docs/ROADMAP.md` beschrijft de ontwikkelfases.

De kaarttegels bevatten terrein, padstatus en een gebouwtype. Bezoekers worden als compacte waarden in de simulatie bewaard. De simulatie draait op vaste stappen van 1/30 seconde; de renderer vraagt schermsynchronisatie aan om de beelden gelijkmatig te presenteren.

De code bevat momenteel geen opgeslagen scenario's, JSON-contentbestanden, echte afbeeldingssprites of save/load-systeem. De texturemanager is aanwezig voor toekomstige BMP-assets; de huidige beelden worden volledig met code getekend.
