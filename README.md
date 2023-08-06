# K-CTP
In this paper, we study online algorithms for the Canadian Traveller Problem defined by Papadimitriou and Yannakakis in 1991. This problem involves a traveller who knows the entire road network in advance, and wishes to travel as quickly as possible from a source vertex 0 to a destination vertex n-1, but discovers online that some roads are blocked (e.g., by snow) once reaching them. Achieving a bounded competitive ratio for the problem is PSPACE-complete. The maximum number of locks (k) is known to him in advance.

During the project, we implemented and tested the most popular and effective polynomial and pseudo-polynomial algorithms to achieve the result closest to optimal. And also implemented a flexible and efficient C++ system in which you can conveniently add and test new strategies.

### For developers
To add new strategy X:
- Add XStrategy class to strategies.
- Inherit from BaseStrategy and overload private travel method.
- Use functions from BaseStrategy and Utiles/Graph to make it easier.
- Check RepositionStrategy and NDPStrategy as en example of deterministic and randomized strategy.

Similarly , when implementing a new opponent.

To test your strategy call it from main.cpp
