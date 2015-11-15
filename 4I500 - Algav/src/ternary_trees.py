"""Module de gestion des tries hybrides. TernarySearchTree est la classe
associée, et des fonctions de gestion existe afin de manipuler l'arbre de manière
non POO."""

import string

EXEMPLE = "A quel genial professeur de dactylographie sommes-nous redevables de \
la superbe phrase ci-dessous, un modele du genre, que toute dactylo connait par \
coeur puisque elle fait appel a chacune des touches du clavier de la machine a ecrire ?"

def only_alpha():
    """Convert unicode string to ascii string without punctuations."""
    rstring = ""
    for word in EXEMPLE:
        for letter in word:
            if (letter in string.ascii_lowercase) or \
            (letter in string.ascii_uppercase) or \
            (letter == ' ') or (letter == '-'):
                rstring += letter
    return rstring

EXAMPLE = only_alpha()

class TernarySearchTree(object):
    """Represent a dictionnary, which as a ternary Search tree."""
    def __init__(self, word=None):
        self.left = None
        self.equal = None
        self.right = None
        self.final = False
        self.key = None

        if word is not None:
            self.add_word(word.lower())

    def add_word(self, word):
        """Add a word to the tree."""
        word = word.lower()
        if len(word) == 0:
            return None

        if self.key is None: # If we are on the root uninitialized.
            self.key = word[0] # Initialize.
            if len(word) == 1:
                self.final = True
            else:
                self.equal = TernarySearchTree(word[1:])

        # Tree already full.
        elif word[0] == self.key:
            if len(word) == 1: # One letter word.
                self.final = True
            # Existent or non-existent son ? Yes => Add, No => Create.
            elif self.equal is None:
                self.equal = TernarySearchTree(word[1:])
            else:
                self.equal.add_word(word[1:])
        elif word[0] < self.key:
            # Existent or non-existent son ? Yes => Add, No => Create.
            if self.left is None:
                self.left = TernarySearchTree(word)
            else:
                self.left.add_word(word)
        else: # If first letter > key
            # Existent or non-existent son ? Yes => Add, No => Create.
            if self.right is None:
                self.right = TernarySearchTree(word)
            else:
                self.right.add_word(word)

    def is_empty(self):
        """Test if the tree is empty."""
        if self.left is None and self.equal is None and self.right is None and self.key is None:
            return True
        return False

    def contains(self, word):
        """Test if tree contains the word."""
        word = word.lower()
        # Word is empty or tree is empty.
        if len(word) == 0:
            return True
        if self.key is None:
            return False

        # Look for the words in other branches.
        if word[0] < self.key and self.left is not None:
            return self.left.contains(word)
        elif word[0] > self.key and self.right is not None:
            return self.right.contains(word)
        elif word[0] == self.key: # On the correct branch.
            if len(word) == 1:
                if self.final is True:
                    return True
                else:
                    return False
            if self.equal is not None:  # Search for the rest of word.
                return self.equal.contains(word[1:])
            return False
        else:
            return False

    def number_words(self):
        """Return the number of words. Just search for final tag."""
        number = 0
        if self.left is not None:
            number += self.left.number_words()
        if self.equal is not None:
            number += self.equal.number_words()
        if self.right is not None:
            number += self.right.number_words()
        if self.final is True:
            number += 1
        return number

    def all_words(self):
        """Return all the words in the tree."""
        words = []
        def get_all(tree, buffer=''):
            """Return all the words in the tree."""
            if tree.key is None:
                return None
            if tree.left is not None: # Left branch, lower than word[0].
                get_all(tree.left, buffer)

            if tree.final is True: # On node.
                words.append(buffer + tree.key)
            if tree.equal is not None: # Equal branch, keep the buffer and the letter.
                get_all(tree.equal, buffer + tree.key)

            if tree.right is not None: # Right branch, higher than word[0]
                get_all(tree.right, buffer)
        get_all(self)
        return words

    def height(self):
        """Get the height of the tree."""
        if self.key is None:
            return 0
        number = 0
        if self.left != None:
            temp = self.left.height()
            if temp > number:
                number = temp
        if self.equal is not None:
            temp = self.equal.height()
            if temp > number:
                number = temp
        if self.right is not None:
            temp = self.right.height()
            if temp > number:
                number = temp
        return number + 1

    def average_height(self):
        """Get the average height of the tree."""
        average = 0.0
        if self.left is not None:
            average = self.left.average_height()
        if self.equal is not None:
            temp = self.equal.average_height()
            if average == 0.0:
                average = temp
            else:
                average = round((average + temp) / 2, 2)
        if self.right is not None:
            temp = self.right.average_height()
            if average == 0.0:
                average = temp
            else:
                average = round((average + temp) / 2, 2)
        return round(average + 1)

    def prefix(self, word):
        """Get all words which prefix is word."""
        word = word.lower()

        def create_map(pref):
            """Factory to map a list[str] and add prefix."""
            def add_pref(word):
                """Return word with pref added."""
                return pref + word
            return add_pref

        answer = []
        def get_all(tree, word, buffer=''):
            """Get all words which prefix is word."""
            word = word.lower()
            if tree is None:
                return []

            if len(word) == 1: # We're on the node of the end of prefix.
                buffer += tree.key
                if tree.final is True: # Prefix is a valid word.
                    print(buffer)
                    answer.append(buffer)
                if tree.equal is not None: # Get all the remaining words.
                    words = tree.equal.all_words()
                    # Map the list to get the correct words.
                    return list(map(create_map(buffer), words)) + answer
                return answer

            if tree.key == word[0]: # The prefix is correct, continue to find next.
                if tree.equal is not None:
                    return get_all(tree.equal, word[1:], buffer + tree.key)
            if tree.key < word[0]: # The letter is incorrect, search for prefix.
                if tree.left is not None:
                    return get_all(tree.left, word, buffer)
            if tree.key > word[0]: # The letter is incorrect, search for prefix.
                if tree.right is not None:
                    return get_all(tree.right, word, buffer)
            return answer
        return get_all(self, word)

    def suppress(self, word):
        """Suppress the word from the tree."""
        word = word.lower()
        if len(word) == 0: # Impossible, so return False.
            return None

        if self.key == word[0]: # Equal.
            if len(word) == 1: # On a leaf.
                if self.right is None and self.left is None and self.equal is None:
                    if self.final is True: # If the word exists.
                        self.final = False
                        return True # Suppress
                    return None
                else:
                    self.final = False # Suppress but keep structure.
                    return None

            if self.equal is not None:
                suppr = self.equal.suppress(word[1:]) # Recursively suppress.
                if suppr is True:
                    self.equal = None # Suppress son.
                if self.right is None and self.left is None and self.equal is None:
                    # If letter is terminal, we could suppress another word.
                    if self.final is False:
                        return True
                return None
        elif self.key < word[0]: # Recursively suppress.
            if self.right != None:
                self.right.suppress(word)
            return None
        elif self.key > word[0]: # Recursively suppress.
            if self.left != None:
                self.left.suppress(word)
            return None
        else:
            return None

    def spaces(self, number):
        """Add a string representation."""
        rstring = " " * number + str(self.final) + " " + str(self.key) + "\n"
        if self.left is not None:
            rstring += self.left.spaces(number + 2)
        if self.equal is not None:
            rstring += self.equal.spaces(number + 2)
        if self.right is not None:
            rstring += self.right.spaces(number + 2)
        return rstring
    def __repr__(self):
        return self.spaces(0)

# Functions to comply specifications...
def Recherche(tree, word):
    """Recherche le mot word dans l'arbre tree et indique s'il est dans l'arbre."""
    return tree.contains(word)
def ComptageMot(tree):
    """Retourne le nombre de mots de l'arbre tree."""
    return tree.number_words()
def ListeMots(tree):
    """Liste tous les mots de l'arbre tree."""
    return tree.all_words()
def ComptageNil(tree):
    """Compte le nombre de pointeurs nuls de l'arbre tree."""
    if tree is None:
        return 1
    number = 0
    if tree.is_empty:
        return 4
    number += ComptageNil(tree.left)
    number += ComptageNil(tree.equal)
    number += ComptageNil(tree.right)
    return number
def Hauteur(tree):
    """Retourne la hauteur de l'arbre tree."""
    return tree.height()
def ProfondeurMoyenne(tree):
    """Retourne la hauteur moyenne de l'arbre tree."""
    return tree.average_height()
def Prefixe(tree, word):
    """Retourne tous les mots de l'arbre tree commençant par le préfixe word."""
    return tree.prefix(word)
def Suppression(tree, word):
    """Supprime le mot word de l'arbre tree s'il existe."""
    return tree.suppress(word)