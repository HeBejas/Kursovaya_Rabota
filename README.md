# Kursovaya_Rabota

Цель курсовой работы: проведение сравнительного анализа выбранных алгоритмов, включая теоретическое исследование их вычислительной сложности, практическую реализацию и экспериментальное тестирование производительности.

Существует ряд классических алгоритмов для решения задачи поиска кратчайшего пути, среди которых наиболее известны: алгоритм Дейкстры, алгоритм Беллмана–Форда, алгоритм Флойда–Уоршелла, алгоритм A* (A-star) и алгоритм Джонсона. В данной работе для сравнительного анализа выбраны алгоритм Дейкстры и алгоритм A*. Такой выбор обусловлен тем, что оба алгоритма предназначены для поиска кратчайшего пути от одной вершины к другой в графах с неотрицательными весами рёбер, однако принципиально различаются по подходу: Дейкстра является алгоритмом «слепого» поиска, тогда как A* использует эвристическую оценку для направления поиска, что потенциально обеспечивает большую эффективность. В данном репозитории предоставлен код обоих алгоритмов.

Алгоритм Дейкстры

Псевдокод:

    function Dijkstra(graph, source):
        create vertex set Q
        for each vertex v in Graph:
            dist[v] ← INFINITY
            prev[v] ← UNDEFINED
            add v to Q
        dist[source] ← 0
    while Q is not empty:
        u ← vertex in Q with min dist[u]
        remove u from Q
        for each neighbor v of u:
            alt ← dist[u] + length(u, v)
            if alt < dist[v]:
                dist[v] ← alt
                prev[v] ← u

    return dist[], prev[]
    
Входные данные:

  Граф (Graph) - Матрица смежности: Двумерный массив, где graph[u][v] содержит вес ребра из u в v, или специальное значение (например, 0 или INF), если ребра не существует.
  
  Вершина-источник (source). Целое число, идентифицирующий начальную вершину.
  
Выходные данные:

  dist[]: массив длин кратчайших путей от источника до всех вершин
  
  prev[]: массив предков для восстановления маршрутов

Алгоритм A* (A-Star)

Псевдокод:

    function AStar(start, goal, graph, h):
        openSet ← {start}
        gScore ← map with default value INFINITY
        gScore[start] ← 0
        fScore ← map with default value INFINITY
        fScore[start] ← h(start, goal)
        cameFrom ← empty map
    
        while openSet is not empty:
            current ← vertex in openSet with lowest fScore
            if current == goal:
                return reconstruct_path(cameFrom, current)
            
            remove current from openSet
            for each neighbor of current:
                tentative_gScore ← gScore[current] + graph[current][neighbor]
                if tentative_gScore < gScore[neighbor]:
                    cameFrom[neighbor] ← current
                    gScore[neighbor] ← tentative_gScore
                    fScore[neighbor] ← gScore[neighbor] + h(neighbor, goal)
                    if neighbor not in openSet:
                        openSet ← openSet ∪ {neighbor}
        
        return failure
        
Входные данные:

    Граф (graph): матрица смежности n×n (аналогично Дейкстре)
    
    Старт (start): целое число 0..n-1 - начальная вершина
    
    Цель (goal): целое число 0..n-1 - конечная вершина
    
    Эвристика (h): функция h(u, goal), оценивающая расстояние от u до goal
    
Выходные данные

    Путь: список вершин от start до goal или пустой список если путь не найден
    
Компилятор:

msys2-arm64-20250830.exe
