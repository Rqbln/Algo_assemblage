# Projet Théorie des Graphes - Optimisation d'une Ligne d'Assemblage
## ING2 2023-2024, Semestre 1

Ce projet se concentre sur l'optimisation d'une ligne d'assemblage en utilisant la théorie des graphes. Le cas d'étude est la ligne d'assemblage de la voiture électrique ECElecar, mais le logiciel développé est adaptable à n'importe quelle ligne d'assemblage.

### Table des Matières
1. [Contexte](#1-contexte)
2. [Cahier des Charges](#2-cahier-des-charges)
3. [Organisation](#3-organisation)
4. [Utilisation de GLPK](#4-utilisation-de-glpk)

### 1. Contexte
Le projet vise à optimiser les processus d'une ligne d'assemblage, en prenant en compte diverses contraintes telles que l'exclusion, la précédence, et le temps de cycle. Les principaux aspects à considérer sont :
- **Contraintes d'exclusion** : Certaines opérations ne peuvent pas être réalisées par la même station.
- **Contraintes de précédence** : Certaines opérations doivent être effectuées avant d'autres.
- **Temps de cycle** : Durée fixe pour chaque station, identique pour toutes.

### 2. Cahier des Charges
Le cahier des charges détaille l'approche méthodologique pour répondre aux différentes contraintes :
- **Première approche simpliste** : Traiter chaque contrainte indépendamment.
- **Approche multi-contraintes** : Combinaison des contraintes pour une optimisation plus complexe.
- **Extension : heuristique** : Utilisation d'heuristiques pour une résolution efficace des contraintes combinées.

Les données seront stockées dans des fichiers texte avec une architecture spécifique :
- `exclusions.txt`
- `precedences.txt`
- `operations.txt`
- `temps_cycle.txt`

### 3. Organisation
Le projet a été réalisé en langage C. Les aspects organisationnels importants sont :
- Utilisation de **Git** pour la gestion du code source.
- Rendu des **livrables** : diaporama de soutenance et dépôt du projet sur GitHub Classroom.

### 4. Utilisation de GLPK
Pour gérer les contraintes d'exclusion, nous avons utilisé GLPK (GNU Linear Programming Kit). Voici un aperçu du processus :
- **Configuration de GLPK** : Nous avons configuré GLPK pour activer les messages pour `glp_simplex` et garder les messages désactivés pour `glp_intopt`.
- **Création des Variables de Décision** : Chaque opération dans une station est représentée par une variable de décision.
- **Contraintes d'Exclusion** : Nous avons mis en place des contraintes pour s'assurer que certaines opérations ne sont pas réalisées par la même station.
- **Contraintes de Temps de Cycle** : Nous avons ajouté des contraintes pour que la somme des durées des opérations dans une station ne dépasse pas le temps de cycle défini.
- **Résolution du Problème** : Le problème a été résolu en utilisant à la fois `glp_simplex` pour les problèmes linéaires et `glp_intopt` pour les optimisations entières mixtes.
