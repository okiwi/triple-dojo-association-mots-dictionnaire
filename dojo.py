#!/usr/bin/env python3
# http://codekata.com/kata/kata08-conflicting-objectives/

flux_brut = open("dico.txt")
flux_épuré = map(str.strip, flux_brut)
dictionnaire = set(flux_épuré)

def a_six_lettres(mot):
	return len(mot) == 6

mots_de_six_lettres = filter(a_six_lettres, dictionnaire)

intervalle = range(1, 6)
for mot in mots_de_six_lettres:
	for curseur in intervalle:
		préfixe = mot[:curseur]
		suffixe = mot[curseur:]
		if (préfixe in dictionnaire and suffixe in dictionnaire):
			print(préfixe, "+", suffixe, "->", mot)


