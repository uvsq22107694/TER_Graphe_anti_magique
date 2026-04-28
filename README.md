# TER_Graphe_anti_magique
Projet de Master TER sur les graphes anti magique

Voici le [lien vers le rapport du TER](https://docs.google.com/document/d/1bdYa86l7v_QxOmbMvtDrT6phl8HoHgJxTnwH-GGkV0E/edit?usp=sharing)

### Création des hypergraphes k-partis (k, n)

L'algorithme implémenté construit un hypergraphe complet k-parti de manière itérative afin d'assurer et de maintenir sa propriété anti-magique à chaque étape. Le processus repose sur trois phases principales :

1. **Cas de base (k=2)** : L'algorithme initialise un hypergraphe biparti où chaque partition contient `n` sommets. Les hyper-arêtes sont générées en suivant un ordre strictement lexicographique (par exemple : `{A1, B1} = 1`, `{A1, B2} = 2`, etc.).
2. **Ajout de partitions par duplication** : L'hypergraphe est étendu une partition à la fois. Pour ajouter un groupe de `n` sommets, chaque arête existante est dupliquée `n` fois, et on y associe séquentiellement chacun des nouveaux sommets, préservant ainsi l'ordre lexicographique naturel.
3. **Maintien de la propriété anti-magique** : Immédiatement après la création de la base et après chaque ajout de partition, l'algorithme teste si le graphe est anti-magique (c'est-à-dire si la somme des labels des arêtes est unique pour chaque sommet). Si ce n'est pas le cas, l'algorithme opère une correction ciblée dans la matrice d'incidence : il intervertit la dernière arête du graphe avec l'arête qui contient le sommet du milieu de la dernière partition. Cette technique résout efficacement les conflits de sommes et garantit un étiquetage anti-magique valide pour le graphe final.