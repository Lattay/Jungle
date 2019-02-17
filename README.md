# A life simulator
This is a life simulator based on basic rules but not too basic.
This project is for experimentation purpose and I am very curious of
what will appear at the end.

# Current state of the project

The project is still in its very early stage where I did not implemented all the basics
features and I am still designing some mechanisms. That is why there is noot even a Makefile.

# Principles

This program simulate the evolution of a population of individuals interacting
with each others.

All individuals evolve on a finite 2D field where they can move, live and die.

The simulation is organized around three scales:
- species: a species define a group of individuals that can reproduce themselves
- families: a family define a group of individuals from the same species that strictly share the same features
- individuals: individuals are points on the field moving and interacting with others

There are four features that define a family of individuals.
They are real numbers between 0 and 1.
Two defines the interactions:
- Agressiveness
- Sociability
Two others define the life cycle:
- Vitality
- Fertility

The interactions are very simple:
- Individual of the same species are attracted to each others proportionaly to their Sociability
- Individual of different species assault each other when they reach a minimum distance
defined by Agressiveness

When assaulted an individual loose a bit of Vitality.
When assaulting an individual win a bit of ressources for its family.

After a fixed amount of time a life cycle occur.
Individual have a probability to die depending on their vitality.
Randomly selected families are crossed with randomly selected other family of the same species
to create a new family of newborns. The population of the new family is determined thanks
to a Verhulst like model.
New borns features are obtained by crossing two families of the same species which create a new distinct family.

The field provide a constant amount of ressources which means that if the population grow too fast
it will be restrained by the lack of ressources.

# Unit testing

The code will ship with a suite of unit tests to check its validy. The test suite
is based on the [Unity framework](https://github.com/ThrowTheSwitch/Unity).
