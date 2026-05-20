#include "Pathfindings.h"
#include "Directions.h"
#include "StructNodes.h"
#include "Element.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <iostream>

std::vector<Position> Pathfinding::ReconstructorPath(
    Position (*predecessors)[GRID_COLS],
    const Position &start,
    const Position &destiny)
{
    std::vector<Position> shortestPath;
    Position currentNode = destiny;
    std::cout << "Reconstruyendo path desde: " << destiny.i << "," << destiny.j
              << " hasta: " << start.i << "," << start.j << std::endl;
    while (currentNode != Position{-1, -1})
    {
        shortestPath.push_back(currentNode);
        currentNode = predecessors[currentNode.i][currentNode.j];

        // aquí currentNode ya es el nuevo valor
        // si es inválido pero no es el sentinel, algo salió mal
        if (IsInvalidPosition(currentNode) && currentNode != Position{-1, -1})
        {
            std::cout << "Predecesor genuinamente invalido" << std::endl;
            shortestPath.clear();  // path corrupto, mejor retornar vacío
            return shortestPath;
        }
    }

    // agregar el nodo inicio
    shortestPath.push_back(start);

    std::reverse(shortestPath.begin(), shortestPath.end());
    return shortestPath;
}

int Pathfinding::Heuristic(const Position &start, const Position &destiny)
{
    Position manhattan = start - destiny;
    return manhattan.i + manhattan.j;
}

std::vector<Position> Pathfinding::Bfs(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny)
{
    std::cout << "BFS desde: " << start.i << "," << start.j
              << " hasta: " << destiny.i << "," << destiny.j << std::endl;

    // si origen == destino
    if (start == destiny)
    {
        std::cout << "Origen == destino" << std::endl;
        return std::vector<Position>();
    }

    std::deque<Position> queue;
    queue.push_back(start);

    Position predecessors[GRID_ROWS][GRID_COLS];
    bool visited[GRID_ROWS][GRID_COLS];

    std::fill(&predecessors[0][0], &predecessors[0][0] + (GRID_ROWS * GRID_COLS), Position{-1, -1});
    std::fill(&visited[0][0], &visited[0][0] + (GRID_ROWS * GRID_COLS), false);

    predecessors[start.i][start.j] = Position{-1, -1};
    visited[start.i][start.j] = true;

    while (!queue.empty())
    {
        Position currentNode = std::move(queue.front());
        queue.pop_front();

        if (currentNode == destiny)
            return ReconstructorPath(predecessors, start, destiny);

        for (auto direction : directions)
        {
            Position neighbor = currentNode + direction;
            if (!IsInvalidPosition(neighbor) && (map[neighbor.i][neighbor.j]->GetType() == TypeElement::Floor) && !visited[neighbor.i][neighbor.j]) // ← faltaba el !
            {
                visited[neighbor.i][neighbor.j] = true;
                queue.push_back(neighbor);
                predecessors[neighbor.i][neighbor.j] = currentNode;
            }
        }
    }
    std::cout << "BFS no encontro camino" << std::endl;
    return std::vector<Position>();
}

void Pathfinding::FloodFill(Element *(&map)[GRID_ROWS][GRID_COLS], int (&zones)[GRID_ROWS][GRID_COLS], const Position &start, int actualColor)
{
    std::deque<Position> queue;
    queue.push_back(start);
    zones[start.i][start.j] = actualColor;
    while (!queue.empty())
    {
        Position currentNode = std::move(queue.front());
        queue.pop_front();

        for (auto direction : directions)
        {
            Position neighbor = currentNode + direction;
            if (IsInvalidPosition(neighbor))
                continue;
            if (map[neighbor.i][neighbor.j] == nullptr)
                continue; // ← agregar esto
            if (map[neighbor.i][neighbor.j]->GetType() != TypeElement::Floor)
                continue;
            if (zones[neighbor.i][neighbor.j] != 0)
                continue;

            zones[neighbor.i][neighbor.j] = actualColor;
            queue.push_back(neighbor);
        }
    }
}

bool Pathfinding::IsConected(Element *(&map)[GRID_ROWS][GRID_COLS], const Position destinies[8])
{
    int colorsMap[GRID_ROWS][GRID_COLS];
    std::fill(&colorsMap[0][0], &colorsMap[0][0] + (GRID_ROWS * GRID_COLS), 0);
    int actualColor = 1;
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
            if (map[i][j] == nullptr)
                continue; // ← agregar antes del GetType
            if (map[i][j]->GetType() == TypeElement::Floor && colorsMap[i][j] == 0){
                FloodFill(map, colorsMap, Position{i, j}, actualColor);
                actualColor++;
            }
        }
    }
    std::unordered_map<int, int> colorCounts;

    for (int i = 0; i < 8; i++)
    {
        std::unordered_set<int> currentCOlors;

        for (auto direction : directions)
        {
            Position tmp = destinies[i] + direction;
            if (IsInvalidPosition(tmp))
                continue;

            int colorVecino = colorsMap[tmp.i][tmp.j];
            if (colorVecino != 0)
            {
                currentCOlors.insert(colorVecino);
            }
        }

        for (int color : currentCOlors)
        {
            colorCounts[color]++;
        }
    }

    for (auto const &[color, count] : colorCounts)
    {
        if (count == 8)
        {
            return true;
        }
    }

    return false;
}

std::vector<Position> Pathfinding::Dijkstra(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny)
{
      std::cout << "Dijkstra desde: " << start.i << "," << start.j 
              << " hasta: " << destiny.i << "," << destiny.j << std::endl;
    std::priority_queue<Node> pq;
    pq.push(Node{start, 0});

    Position predecessors[GRID_ROWS][GRID_COLS];
    int costSoFar[GRID_ROWS][GRID_COLS];
    bool visited[GRID_ROWS][GRID_COLS];

    std::fill(&predecessors[0][0], &predecessors[0][0] + (GRID_ROWS * GRID_COLS), Position{-1, -1});
    std::fill(&costSoFar[0][0], &costSoFar[0][0] + (GRID_ROWS * GRID_COLS), INF);
    std::fill(&visited[0][0], &visited[0][0] + (GRID_ROWS * GRID_COLS), false);

    predecessors[start.i][start.j] = Position{-1, -1};
    costSoFar[start.i][start.j] = 0;

    while (!pq.empty())
    {
        Node currentNode = pq.top();
        pq.pop();

        // ← marcar como visitado al sacar de la cola
        if (visited[currentNode.pos.i][currentNode.pos.j])
            continue;
        visited[currentNode.pos.i][currentNode.pos.j] = true;

        if (currentNode.pos == destiny)
            return ReconstructorPath(predecessors, start, destiny);

        for (auto direction : directions)
        {
            Position neighborPos = currentNode.pos + direction;

            if (IsInvalidPosition(neighborPos))
                continue;
            if (map[neighborPos.i][neighborPos.j] == nullptr)
                continue; // ← agregar
            if (map[neighborPos.i][neighborPos.j]->GetType() != TypeElement::Floor)
                continue;
            if (visited[neighborPos.i][neighborPos.j])
                continue;

            int newCost = costSoFar[currentNode.pos.i][currentNode.pos.j] + map[neighborPos.i][neighborPos.j]->GetWeightGame();

            if (newCost < costSoFar[neighborPos.i][neighborPos.j])
            {
                costSoFar[neighborPos.i][neighborPos.j] = newCost;
                pq.push(Node{neighborPos, newCost});
                predecessors[neighborPos.i][neighborPos.j] = currentNode.pos;
            }
        }
    }
    std::cout << "Dijkstra no encontro camino" << std::endl;
    return std::vector<Position>();
}

std::vector<Position> Pathfinding::A_Star_Game(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny)
{
    std::priority_queue<Node> pq;
    pq.push(Node{start, Heuristic(start, destiny)});

    Position predecessors[GRID_ROWS][GRID_COLS];
    int costSoFar[GRID_ROWS][GRID_COLS];
    bool visited[GRID_ROWS][GRID_COLS];

    std::fill(&predecessors[0][0], &predecessors[0][0] + (GRID_ROWS * GRID_COLS), Position{-1, -1});
    std::fill(&costSoFar[0][0], &costSoFar[0][0] + (GRID_ROWS * GRID_COLS), INF);
    std::fill(&visited[0][0], &visited[0][0] + (GRID_ROWS*GRID_COLS), false);

    predecessors[start.i][start.j] = Position{-1, -1};
    costSoFar[start.i][start.j] = 0;

    while (!pq.empty())
    {
        Node currentNode = pq.top();
        pq.pop();

        if(visited[currentNode.pos.i][currentNode.pos.j]) continue;
        visited[currentNode.pos.i][currentNode.pos.j] = true;

        if (currentNode.pos == destiny)
        {
            return ReconstructorPath(predecessors, start, destiny);
        }

        for (auto direction : directions)
        {
            Position neighborPos = currentNode.pos + direction;

            if (map[neighborPos.i][neighborPos.j] == nullptr)
                continue;

            if (IsInvalidPosition(neighborPos) || map[neighborPos.i][neighborPos.j]->GetType() != TypeElement::Floor)
                continue;

            int newCost = costSoFar[currentNode.pos.i][currentNode.pos.j] + map[neighborPos.i][neighborPos.j]->GetWeightGame();

            int cost = costSoFar[neighborPos.i][neighborPos.j];

            if (cost == INF || newCost < cost)
            {
                costSoFar[neighborPos.i][neighborPos.j] = newCost;
                int priority = newCost + Heuristic(neighborPos, destiny);
                pq.push(Node{neighborPos, priority});
                predecessors[neighborPos.i][neighborPos.j] = currentNode.pos;
            }
        }
    }

    return std::vector<Position>();
}

std::vector<Position> Pathfinding::A_Star_Map(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny)
{
    std::priority_queue<Node> pq;
    pq.push(Node{start, Heuristic(start, destiny)});

    std::vector<Position> obstacles;

    Position predecessors[GRID_ROWS][GRID_COLS];
    int costSoFar[GRID_ROWS][GRID_COLS];
    bool visited[GRID_ROWS][GRID_COLS];

    std::fill(&predecessors[0][0], &predecessors[0][0] + (GRID_ROWS * GRID_COLS), Position{-1, -1});
    std::fill(&costSoFar[0][0], &costSoFar[0][0] + (GRID_ROWS * GRID_COLS), INF);
    std::fill(&visited[0][0], &visited[0][0] + (GRID_ROWS * GRID_COLS), false);

    predecessors[start.i][start.j] = Position{-1, -1};
    costSoFar[start.i][start.j] = 0;

    int weightDestiny = 1;

    while (!pq.empty())
    {
        Node currentNode = pq.top();
        pq.pop();

        if(visited[currentNode.pos.i][currentNode.pos.j]) continue;
        visited[currentNode.pos.i][currentNode.pos.j] = true;

        if (currentNode.pos == destiny)
        {
            for (Position p : ReconstructorPath(predecessors, start, destiny))
            {
                if (map[p.i][p.j]->GetType() != TypeElement::Obstacle)
                    continue;

                obstacles.push_back(p);
            }

            return obstacles;
        }

        for (auto direction : directions)
        {
            Position neighborPos = currentNode.pos + direction;

            if (IsInvalidPosition(neighborPos))
                continue;

            int newCost = 0;

            if (neighborPos == destiny){
                newCost = costSoFar[currentNode.pos.i][currentNode.pos.j] + weightDestiny;
            }

            else{
                newCost = costSoFar[currentNode.pos.i][currentNode.pos.j] + map[neighborPos.i][neighborPos.j]->GetWeightMap();
            }

            int cost = costSoFar[neighborPos.i][neighborPos.j];

            if (cost == INF || newCost < cost)
            {
                costSoFar[neighborPos.i][neighborPos.j] = newCost;
                int priority = newCost + Heuristic(neighborPos, destiny);
                pq.push(Node{neighborPos, priority});
                predecessors[neighborPos.i][neighborPos.j] = currentNode.pos;
            }
        }
    }

    return std::vector<Position>();
}

bool Pathfinding::IsInvalidPosition(const Position &neighborPos)
{
    return neighborPos.i < 0 || neighborPos.i >= GRID_ROWS || neighborPos.j < 0 || neighborPos.j >= GRID_COLS;
}

void Pathfinding::RandomMoves(Element *(&map)[GRID_ROWS][GRID_COLS], std::vector<Position> &pos, const Position &start)
{
    Position tmp = start;
    int moves = 0;
    std::random_device rng{};
    std::mt19937 gen{rng()};
    std::uniform_int_distribution<int> move{0, 3};

    while (moves < 10)
    {
        Position prev = tmp;
        int selection = move(gen);
        switch (selection)
        {
        case 0:
            tmp += up;
            break;
        case 1:
            tmp += down;
            break;
        case 2:
            tmp += right;
            break;
        case 3:
            tmp += left;
            break;
        }

        if (IsInvalidPosition(tmp) || map[tmp.i][tmp.j]->GetType() != TypeElement::Floor)
        {
            tmp = prev;
            moves++;
            continue;
        }
        pos.push_back(tmp);
        moves++;
    }
}

std::vector<Position> Pathfinding::LOS(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny)
{
    Position distance = start - destiny;
    std::cout<<"La distancia entre: "<<start.i<<','<<start.j<<" y "<<destiny.i<<','<<destiny.j<<" es igual a "<<distance.i<<','<<distance.j<<'\n';
    Position tmp = start;
    Position directionToMoveRows = (start.i > destiny.i) ? up : down;
    Position directionToMoveCols = (start.j > destiny.j) ? left : right;
    std::vector<Position> positions;
    for (int i = 0; i < distance.i; i++)
    {
        tmp += directionToMoveRows;
        if ((IsInvalidPosition(tmp) || map[tmp.i][tmp.j]->GetType() != TypeElement::Floor) && tmp != destiny)
        {
            return std::vector<Position>();
        }

        positions.push_back(tmp);
    }

    for (int i = 0; i < distance.j; i++)
    {
        tmp += directionToMoveCols;
        if ((IsInvalidPosition(tmp) || map[tmp.i][tmp.j]->GetType() != TypeElement::Floor) && tmp != destiny)
        {
            return std::vector<Position>();
        }

        positions.push_back(tmp);
    }

    return positions;
}

std::vector<Position> Pathfinding::AleatoryMovement(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny)
{
  std::cout << "AleatoryMovement desde: " << start.i << "," << start.j 
              << " hasta: " << destiny.i << "," << destiny.j << std::endl;
    
    std::vector<Position> path = LOS(map, start, destiny);
    std::cout << "LOS path size: " << path.size() << std::endl;

    if(!path.empty()) return path;

    RandomMoves(map, path, start);
    std::cout << "RandomMoves path size: " << path.size() << std::endl;

    if(!path.empty()){
        Position newStart = path.back();
        std::vector<Position> temp2 = LOS(map, newStart, destiny);
        
        if(!temp2.empty()){
            path.insert(path.end(), temp2.begin(), temp2.end());
            return path;
        }
    }
    
    std::cout << "AleatoryMovement fallo, usando BFS como fallback" << std::endl;
    return Bfs(map, start, destiny);
}

std::vector<Position> Pathfinding::SelectAlgorithm(
    TypePath path,
    Element *(&map)[GRID_ROWS][GRID_COLS],
    const Position &start,
    const Position &destiny)
{
    switch (path)
    {
    case TypePath::ALEATORYMOVEMENT:
        return AleatoryMovement(map, start, destiny);

    case TypePath::ASTAR_GAME:
        return A_Star_Game(map, start, destiny);

    case TypePath::ASTAR_MAP:
        return A_Star_Map(map, start, destiny);

    case TypePath::BFS:
        return Bfs(map, start, destiny);

    case TypePath::DIJKSTRA:
        return Dijkstra(map, start, destiny);

    case TypePath::LOS:
        return LOS(map, start, destiny);

    default:
        return std::vector<Position>();
        break;
    }
}