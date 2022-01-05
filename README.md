# Javni-prevoz
 
 Da biste koristili ovaj kod potrebno je proslediti mu jedan fajl sa stajalištima i jedan fajl sa autobuskim linijama a zatim izabrati operaciju koju zelite da izvršite nad tom mrežom.
 
 Fajl sa stajalištima mora biti u svakom redu imati po jedno stajalište u formatu šifra_stajališta naziv_stajališta.
 Gde je šifra stajališta jedinstven ceo broj.
 
 Fajl sa autobuskim linijama mora u svakom redu imati po jednu autobusku liniju u formatu oznaka_linije [prvi_polazak-poslednji_polazak#razmak_između_polazaka] stajališta.
Gde oznaka linije mora biti jedinstvena, a stajališta su niz šifri stajališta razdvojenih razmakom.

Nakon toga možete iyabrati jednu od 4 opcije:

| Opcija     | Objašnjenje |
| ----------- | ----------- |
1 | Generiše fajl sa šifrom stajališta, nazivom stajališta i svim linijama koje staju na tom stajalištu
2 | Generiše fajl sa oznakom linije prvim i početnim stajalištem te linije i svim šiframa i nazivima stajalista kroz koje prolazi ta linija
3 | Generiše fajl sa oznakom linije, svim linijama koje imaju zajedničko stajalište sa njom i brojem polazaka te linije u jednom danu
4 | Generiše fajl sa najbržom putanjom ili putanjom sa najmanje presedanja izmedju dva stajališta
0 | Prekida izvršavanje programa
