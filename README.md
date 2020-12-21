# Shortest-Path
## Programme 1
To be completed
## Programme 2
Dans le repertoire ~/programme2/ : 
> make 

Puis 

> ./programme2.o 


va créer les fichiers dot et png dans le dossier output/  à partir des listes d'adjacence des graphe construit dans le main du fichier `adjacence_list.c`
### Exemple 1
Construction de la liste d'adjacence d'un  T_graphLA à partir de fichier **`.adj`** qui réprésente la liste sous forme matricielle
### Exemple 2
Construction de la liste d'adjacence d'un  T_graphLA à partir de fichier **`.la`** qui réprésente la liste sous forme de listes d'adjacence dans un format que nous avons défini, soit : 
```
0    1_15    
src  dest_weight dest_weight dest_weight 
src  dest_weight dest_weight
.....
```
Où chaque ligne représente la liste d'adjacence du sommet src.