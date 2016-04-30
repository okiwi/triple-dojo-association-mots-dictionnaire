#!/usr/bin/env python3
# http://codekata.com/kata/kata08-conflicting-objectives/

from sys import stderr
from time import perf_counter


flux_brut = open("dico.txt")
flux_épuré = map(str.strip, flux_brut)
dictionnaire = set(flux_épuré)

def a_six_lettres(mot):
	return len(mot) == 6

mots_de_six_lettres = list(filter(a_six_lettres, dictionnaire))

paires_de_mots = []
intervalle = range(1, 6)

temps_début = perf_counter()

for stuff in range(64):
	for mot in mots_de_six_lettres:
		for curseur in intervalle:
			préfixe = mot[:curseur]
			suffixe = mot[curseur:]
			if (préfixe in dictionnaire and suffixe in dictionnaire):
				paires_de_mots.append((préfixe, suffixe))

temps_fin = perf_counter()
			
for préfixe, suffixe in paires_de_mots:
	print(préfixe, "+", suffixe, "->", préfixe+suffixe)

print("Time:", temps_fin - temps_début, file=stderr)
