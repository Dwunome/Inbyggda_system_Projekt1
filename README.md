# Inbyggda_system_Projekt1

Uppgift: Felsök och korrigera en befintlig C-kod från C89 till C99. 



# Borttagna filer: 
Byte.c, 
Byte.h, 
MemoryBlock.c, 
MemoryBlock.h

# Lade till nya filer: 
Filen Vector.c,
Filen Vector.h,
Filen DynamicTimer.c,
Filen DynamicTimer.h

Dessa filer har implementerats för att vid genomsninttstiden mellan varje knapptryckning skall avgöra hur ofta 
temperaturmätningarna skall ske, via lagring i en dynamisk behållare.

# Resultat
* Programmet fungerar nu som det är tänkt, vid start skrivs meddelandet ”Dynamic temperature
* measurement system”, sedan händer inget förens tryckknappen på PIN 13 trycks ned, se Figur 1. Vid
* första knapptryck kommer ett meddelande som berättar att den dynamiska timern är initierad samt
* att en första avläsning av rumstemperaturen gör, vilket också skrivs ut, se Figur 2.
* Vid nästkommande knapptryckningar skrivs kapacitet och antal element ut, se Figur 3.

* Vid uppstart av programmet så skrivs meddelandet "Dynamic temperature measurement system". Därefter sker ingenting förrän tryckknapp på PIN13 trycks ned.
* Vid första knapptryck skrivs ett meddelande ut som berättar att den dynamiska timern är initierad samt att en temperaturavläsning utförs och skrivs ut.
* Vid nästkommande knapptryckningar skrivs kapacitet, antal element och fördröjningstid mellan temperatur-utskrifterna ut. Därefter sker utskrift av
* temperatur beräknat på genomsnittet av tiden då knappen har tryckts ned.



# Diskussion:
Detta har varit ett utmanande projekt som desvärre hamnade i en pik av flera andra kurser som pågick samtidigt. Detta ledde till ett senare slutförande
av projektet än önskat. Utöver detta så har det varit lärorikt att genomföra projektet i lugn och ro. Ett bra "verktyg" som även togs med var att 
arbeta på en del av koden i taget, samt kompilera ofta för att snabbt upptäcka små fel innan det blir för stora fel.
