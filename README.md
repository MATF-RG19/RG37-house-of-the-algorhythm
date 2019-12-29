# House of the algoRhythm

Video igra inspirisana igricom iz 2014te pod nazivom Crypt of the Necrodancer.
Ritmička, a po malo, i strateška igrica koja tera igrača da u ritmu muzike (u našem slučaju u ritmu
dolaženja plavih linija ka središnjem kvadratu) pomera svog lika i da savlada neprijatelje.

### Pravila igre
* Igrac ima 3 života, ako ih izgubi igra je gotova!
* Ako se igrac pomera u pravom ritmu (kad plava linija stane na crni kvadratić) dobije _score multiplier_.
* Kada igrac ubije slime monstruma (zelene ili plave kockike koje se pomeraju i skakuću) dobije poena u zavisnosti koliko _score multiplier_-a ima.
* Igrač ubija _slime_ monstrume time što se pomera na njihovo polje, ali budite pažljivi, monstrumi mogu takođe da povrede igrača.
* Monstrum povređuje igrača tako što se pomeraju na polje gde je igrač.
* Slime Monstrumi se uvek pomeraju na isti način. 
```diff
@@Plavi _slime_ skakuće gore ili dole@@
```
```diff
+Zeleni _slime_ skakuće levo ili desno.
```
### Komande
|  Taster|Komanda  |
|--------|---------|
| <kbd>w</kbd>  |Kretanje ka gore levo|
| <kbd>s</kbd>  |Kretanje ka dole levo|
| <kbd>a</kbd>  |Kretanje ka dole desno|
| <kbd>d</kbd>  |Kretanje ka gore desno|
 
 
 # Instalacija
 Preuzmie folder sa igricom
 U folderu uradie komande:
 make  
 ./Hota
 
 # Autor
 Nikola Maliković
