from cs50 import get_string
import re


def main():

    text = get_string("Text: ")

    index = colemanLiau(text)

    # Print corresponding grade
    if index < 1:
        print("Before Grade 1")

    elif index > 16:
        print("Grade 16+")

    else:
        print(f"Grade {round(index)}")


def colemanLiau(text):

    # Find number os letters and sentences
    n_sentences = 0
    n_letters = 0
    for char in text:
        if char.isalpha():
            n_letters += 1
        elif char == "." or char == "!" or char == "?":
            n_sentences += 1

    # Find number of words count the number of elementes on the list
    # after we use the split function
    n_words = len(text.split())

    # Find averges
    n_L = (n_letters * 100) / n_words
    n_S = (n_sentences * 100) / n_words

    # Coleman Liau Formula
    index = 0.0588 * n_L - 0.296 * n_S - 15.8

    return index


main()