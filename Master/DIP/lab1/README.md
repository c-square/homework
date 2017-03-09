Laboratorul 1 – funcții elementare de lucru cu imagini
-------------------------------------------------------------

A se consulta fişierele: DIP - Matlab - Fundamentals.pdf, IPtoolbox.pdf

- **Citire**: imread
- **Dimensiune**: size, whos
- **Afişare**: imshow, figure, plot, subplot, imagesc
- **Informații**: iminfo
- **Salvare**: imwrite

Pornind de la imaginea color a unei persoane, să se creeze o imagine binară (pixelii au doar două
valori posibile) de aceeaşi dimensiune, care să scoată în evidență pixelii `piele` din imaginea
inițală. Un pixel în imaginea binară va avea valoarea 0 dacă în imaginea originală pixelul
respectiv nu este pixel `piele` şi 255 dacă este pixel `piele`.

Un pixel (R,G,B) este clasificat ca fiind pixel `piele` dacă:

```
R > 95 && G > 40 && B > 20 &&
max{R, G, B} - min{R, G, B} > 15 &&
|R - G| > 15 && R > G && R > B
```

Pornind de la această imagine binară, să se construiască "negativul" ei.

Folosind clasificarea pixelilor în `piele` - `non-piele` într-o imagine a unei persoane, să se
încerce deducerea unui algoritm de detecție a feței, adică, să se găsească o subimagine pătratică
de dimensiune cât mai mică care să conțină fața persoanei din imaginea originală.

Să se construiască folosind imaginile din arhiva faces94.zip (a se vedea funcția dir) două matrici
male, female în felul următor:

- orice imagine din directorul female/male se redimensionează cu un factor 0.25 (a se
vedea funcția imresize)
- se vectorizează imaginea redimensionată
- vectorul obținut la pasul anterior se pune ca linie a matricii female/male
