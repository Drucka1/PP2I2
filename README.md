# PPII 2

Dans cette branch, j'ai repris ce que Clément avait fait.

Quelques modifications :

- globalement, j'ai décomposé l'ancien main et init pour qu'on s'y retrouve plus facilement.
- j'ai viré le dossier include c'est plus chiant qu'autre chose
- j'ai préféré que les cellules aussi soient malloc, il y a une fonction freeCell.

- les fonctions de gestions de liste d'objets sont désormais void, ça change rien en soit juste plus besoin d'écrire

```
  list = listObjAppend(list, object)

```

juste

```
listObjAppend(list, object)
```

par exemple,

- le "centrage" du joueur est automatique du moment qu'on appelle la fonction movePlayer, cette fonction est appelé par les functions moveRight, moveLeft etc. L'intéret de centrer automiquement permet notemment de simplifier la mise en place de téleporteur.

- pour la génération de map, plus besoin de check si le strlen >= 10 pour récup la map associé à la porte

- une implémentation des objets clés qui couvre plus de possibilités : une clé peut agir sur plusieurs éléments de plusieurs map diférentes
- pareil pour les leviers : la structure permet d'ajouter plus de fonctionnalité au levier comme sa durée d'activation, en fait ça peut être un bouton minecraft.


Bon c'est Léo qui parle du digicode ou crise d'epilepsie code. Le seul probleme c'est l'affichage je fait ça demain par ce qu'il est tard la. le code ressemble a rien me tue pas Cédric stp j'aurais des question sur comment bien le ranger et bien comprendre ton renderer. J'ai fait en sorte qu'il y ai pas de memory leak et que TTF s'init et se quit comme le reste de SDL mais sinon c'est un peu la merde. bref personne va lire ça bonne nuit la tcheam. ps fk sdl. 