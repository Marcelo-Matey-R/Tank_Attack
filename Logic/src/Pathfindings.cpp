#include "Pathfindings.h"
#include "StructPosition.h"
#include "Directions.h"
#include "StructNodes.h"
#include "Element.h"
#include "../../include/MyQueue.h"
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <iostream>

Array<Position> Pathfinding::ReconstructorPath(Position (*predecessors)[GRID_COLS], const Position &start, const Position &destiny){
    Array<Position> shortestPath;
    Position currentNode = destiny;
    std::cout << "Reconstruyendo path desde: " << destiny.i << "," << destiny.j
              << " hasta: " << start.i << "," << start.j << std::endl;
    while (currentNode != Position{-1, -1}){
        shortestPath.PushBack(currentNode);
        std::cout<<"El nodo actual esta en la posicion: "<<currentNode.i<<", "<<currentNode.j<<"\n";
        currentNode = predecessors[currentNode.i][currentNode.j];

        // aquí currentNode ya es el nuevo valor
        // si es inválido pero no es el sentinel, algo salió mal
        if (IsInvalidPosition(currentNode) && currentNode != Position{-1, -1}){
            std::cout << "Predecesor genuinamente invalido" << std::endl;
            shortestPath.Clear();  // path corrupto, mejor retornar vacío
            return shortestPath;
        }
    }

    // agregar el nodo inicio
    //shortestPath.push_back(start);

    std::reverse(shortestPath.begin(), shortestPath.end());
    return shortestPath;
}

int Pathfinding::Heuristic(const Position &start, const Position &destiny){
    Position manhattan = start - destiny;
    return manhattan.i + manhattan.j;
}

Array<Position> Pathfinding::Bfs(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny){
    std::cout << "BFS desde: " << start.i << "," << start.j
              << " hasta: " << destiny.i << "," << destiny.j << std::endl;

    // si origen == destino
    if (start == destiny){
        std::cout << "Origen == destino" << std::endl;
        return Array<Position>();
    }

    Array<Position> queue;
    queue.PushBack(start);

    Position predecessors[GRID_ROWS][GRID_COLS];
    bool visited[GRID_ROWS][GRID_COLS];

    std::fill(&predecessors[0][0], &predecessors[0][0] + (GRID_ROWS * GRID_COLS), Position{-1, -1});
    std::fill(&visited[0][0], &visited[0][0] + (GRID_ROWS * GRID_COLS), false);

    predecessors[start.i][start.j] = Position{-1, -1};
    visited[start.i][start.j] = true;

    while (!queue.IsEmpty())
    {
        Position currentNode = std::move(queue.Front());
        queue.PopFront();

        if (currentNode == destiny)
            return ReconstructorPath(predecessors, start, destiny);

        for (auto direction : directions)
        {
            Position neighbor = currentNode + direction;
            if (!IsInvalidPosition(neighbor) && (map[neighbor.i][neighbor.j]->GetType() == TypeElement::Floor) && !visited[neighbor.i][neighbor.j]) // ← faltaba el !
            {
                visited[neighbor.i][neighbor.j] = true;
                queue.PushBack(neighbor);
                predecessors[neighbor.i][neighbor.j] = currentNode;
            }
        }
    }
    std::cout << "BFS no encontro camino" << std::endl;
    return Array<Position>();
}

void Pathfinding::FloodFill(Element *(&map)[GRID_ROWS][GRID_COLS], int (&zones)[GRID_ROWS][GRID_COLS], const Position &start, int actualColor){
    Array<Position> queue;
    queue.PushBack(start);
    zones[start.i][start.j] = actualColor;
    while (!queue.IsEmpty()){
        Position currentNode = std::move(queue.Front());
        queue.PopFront();

        for (auto direction : directions){
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
            queue.PushBack(neighbor);
        }
    }
}

bool Pathfinding::IsConected(Element *(&map)[GRID_ROWS][GRID_COLS], const Position destinies[8]){
    int colorsMap[GRID_ROWS][GRID_COLS];
    std::fill(&colorsMap[0][0], &colorsMap[0][0] + (GRID_ROWS * GRID_COLS), 0);
    int actualColor = 1;
    for (int i = 0; i < GRID_ROWS; i++){
        for (int j = 0; j < GRID_COLS; j++){
            if (map[i][j] == nullptr)
                continue; // ← agregar antes del GetType
            if (map[i][j]->GetType() == TypeElement::Floor && colorsMap[i][j] == 0){
                FloodFill(map, colorsMap, Position{i, j}, actualColor);
                actualColor++;
            }
        }
    }
    std::unordered_map<int, int> colorCounts;

    for (int i = 0; i < 8; i++){
        std::unordered_set<int> currentCOlors;

        for (auto direction : directions){
            Position tmp = destinies[i] + direction;
            if (IsInvalidPosition(tmp))
                continue;

            int colorVecino = colorsMap[tmp.i][tmp.j];
            if (colorVecino != 0){
                currentCOlors.insert(colorVecino);
            }
        }

        for (int color : currentCOlors){
            colorCounts[color]++;
        }
    }

    for (auto const &[color, count] : colorCounts){
        if (count == 8){
            return true;
        }
    }

    return false;
}

Array<Position> Pathfinding::Dijkstra(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny){
      std::cout << "Dijkstra desde: " << start.i << "," << start.j << " hasta: " << destiny.i << "," << destiny.j << std::endl;
    MiPriorityQueue<Node> pq;
    pq.Enqueue(Node{start, 0}, 0);

    Position predecessors[GRID_ROWS][GRID_COLS];
    int costSoFar[GRID_ROWS][GRID_COLS];
    bool visited[GRID_ROWS][GRID_COLS];

    std::fill(&predecessors[0][0], &predecessors[0][0] + (GRID_ROWS * GRID_COLS), Position{-1, -1});
    std::fill(&costSoFar[0][0], &costSoFar[0][0] + (GRID_ROWS * GRID_COLS), INF);
    std::fill(&visited[0][0], &visited[0][0] + (GRID_ROWS * GRID_COLS), false);

    predecessors[start.i][start.j] = Position{-1, -1};
    costSoFar[start.i][start.j] = 0;

    while (!pq.IsEmpty()){
        Node currentNode = pq.Peek();
        pq.Dequeue();

        // ← marcar como visitado al sacar de la cola
        if (visited[currentNode.pos.i][currentNode.pos.j])
            continue;
        visited[currentNode.pos.i][currentNode.pos.j] = true;

        if (currentNode.pos == destiny)
            return ReconstructorPath(predecessors, start, destiny);

        for (auto direction : directions){
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

            if (newCost < costSoFar[neighborPos.i][neighborPos.j]){
                costSoFar[neighborPos.i][neighborPos.j] = newCost;
                pq.Enqueue(Node{neighborPos, newCost}, newCost);
                predecessors[neighborPos.i][neighborPos.j] = currentNode.pos;
            }
        }
    }
    std::cout << "Dijkstra no encontro camino" << std::endl;
    return Array<Position>();
}

Array<Position> Pathfinding::A_Star_Game(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny){
    MiPriorityQueue<Node> pq;
    pq.Enqueue(Node{start, Heuristic(start, destiny)}, Heuristic(start, destiny));

    Position predecessors[GRID_ROWS][GRID_COLS];
    int costSoFar[GRID_ROWS][GRID_COLS];
    bool visited[GRID_ROWS][GRID_COLS];

    std::fill(&predecessors[0][0], &predecessors[0][0] + (GRID_ROWS * GRID_COLS), Position{-1, -1});
    std::fill(&costSoFar[0][0], &costSoFar[0][0] + (GRID_ROWS * GRID_COLS), INF);
    std::fill(&visited[0][0], &visited[0][0] + (GRID_ROWS*GRID_COLS), false);

    predecessors[start.i][start.j] = Position{-1, -1};
    costSoFar[start.i][start.j] = 0;

    while (!pq.IsEmpty()){
        Node currentNode = pq.Peek();
        pq.Dequeue();

        if(visited[currentNode.pos.i][currentNode.pos.j]) continue;
        visited[currentNode.pos.i][currentNode.pos.j] = true;

        if (currentNode.pos == destiny){
            return ReconstructorPath(predecessors, start, destiny);
        }

        for (auto direction : directions){
            Position neighborPos = currentNode.pos + direction;

            if (map[neighborPos.i][neighborPos.j] == nullptr)
                continue;

            if (IsInvalidPosition(neighborPos) || map[neighborPos.i][neighborPos.j]->GetType() != TypeElement::Floor)
                continue;

            int newCost = costSoFar[currentNode.pos.i][currentNode.pos.j] + map[neighborPos.i][neighborPos.j]->GetWeightGame();

            int cost = costSoFar[neighborPos.i][neighborPos.j];

            if (cost == INF || newCost < cost){
                costSoFar[neighborPos.i][neighborPos.j] = newCost;
                int priority = newCost + Heuristic(neighborPos, destiny);
                pq.Enqueue(Node{neighborPos, priority}, priority);
                predecessors[neighborPos.i][neighborPos.j] = currentNode.pos;
            }
        }
    }

    return Array<Position>();
}

Array<Position> Pathfinding::A_Star_Map(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny){
    MiPriorityQueue<Node> pq;
    pq.Enqueue(Node{start, Heuristic(start, destiny)}, Heuristic(start, destiny));

    Array<Position> obstacles;

    Position predecessors[GRID_ROWS][GRID_COLS];
    int costSoFar[GRID_ROWS][GRID_COLS];
    bool visited[GRID_ROWS][GRID_COLS];

    std::fill(&predecessors[0][0], &predecessors[0][0] + (GRID_ROWS * GRID_COLS), Position{-1, -1});
    std::fill(&costSoFar[0][0], &costSoFar[0][0] + (GRID_ROWS * GRID_COLS), INF);
    std::fill(&visited[0][0], &visited[0][0] + (GRID_ROWS * GRID_COLS), false);

    predecessors[start.i][start.j] = Position{-1, -1};
    costSoFar[start.i][start.j] = 0;

    int weightDestiny = 1;

    while (!pq.IsEmpty()){
        Node currentNode = pq.Peek();
        pq.Dequeue();

        if(visited[currentNode.pos.i][currentNode.pos.j]) continue;
        visited[currentNode.pos.i][currentNode.pos.j] = true;

        if (currentNode.pos == destiny){
            for (Position p : ReconstructorPath(predecessors, start, destiny)){
                if (map[p.i][p.j]->GetType() != TypeElement::Obstacle)
                    continue;

                obstacles.PushBack(p);
            }

            return obstacles;
        }

        for (auto direction : directions){
            Position neighborPos = currentNode.pos + direction;

            if (IsInvalidPosition(neighborPos))
                continue;

            if (visited[neighborPos.i][neighborPos.j])
                continue;

            int newCost = 0;

            if (neighborPos == destiny){
                newCost = costSoFar[currentNode.pos.i][currentNode.pos.j] + weightDestiny;
            }
            else{
                newCost = costSoFar[currentNode.pos.i][currentNode.pos.j] + map[neighborPos.i][neighborPos.j]->GetWeightMap();
            }

            // protección contra overflow: si el costo se desbordó, ignorar
            if (newCost < 0)
                continue;

            int cost = costSoFar[neighborPos.i][neighborPos.j];

            if (cost == (int)INF || newCost < cost){
                costSoFar[neighborPos.i][neighborPos.j] = newCost;
                int priority = newCost + Heuristic(neighborPos, destiny);
                pq.Enqueue(Node{neighborPos, priority}, priority);
                predecessors[neighborPos.i][neighborPos.j] = currentNode.pos;
            }
        }
    }

    return Array<Position>();
}

bool Pathfinding::IsInvalidPosition(const Position &neighborPos){
    return neighborPos.i < 0 || neighborPos.i >= GRID_ROWS || neighborPos.j < 0 || neighborPos.j >= GRID_COLS;
}

void Pathfinding::RandomMoves(Element *(&map)[GRID_ROWS][GRID_COLS], Array<Position> &pos, const Position &start){
    Position tmp = start;
    int moves = 0;
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> move{0, 3};

    while (moves < 10){
        Position prev = tmp;
        int selection = move(gen);
        switch (selection){
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

        if (IsInvalidPosition(tmp) || map[tmp.i][tmp.j]->GetType() != TypeElement::Floor){
            tmp = prev;
            moves++;
            continue;
        }
        pos.PushBack(tmp);
        moves++;
    }
}

bool Pathfinding::MoveToThisDirections(Element *(&map)[GRID_ROWS][GRID_COLS], Array<Position>& positions, Position &start, const int &distance, const Position &directionToMove){
    Position tmp = start; 
    for (int i = 0; i < distance; i++){
        tmp += directionToMove;
        if (IsInvalidPosition(tmp) || map[tmp.i][tmp.j]->GetType() != TypeElement::Floor){
            positions.Clear();
            return false;
        }

        positions.PushBack(tmp);
    }

    start = tmp;
    return true;
}

Array<Position> Pathfinding::LOS(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny){
    Position distance = start - destiny;
    std::cout<<"La distancia entre: "<<start.i<<','<<start.j<<" y "<<destiny.i<<','<<destiny.j<<" es igual a "<<distance.i<<','<<distance.j<<'\n';
    Position tmp = start;
    Position directionToMoveRows = (start.i > destiny.i) ? up : down;
    Position directionToMoveCols = (start.j > destiny.j) ? left : right;
    Array<Position> positions;
    
    //Movimiento vertical --> horizontal
    bool success = MoveToThisDirections(map, positions, tmp, distance.i, directionToMoveRows) && MoveToThisDirections(map, positions, tmp, distance.j, directionToMoveCols);

    // Si falla entonces probar movimiento horizontal --> vertical
    if(!success){
        positions.Clear();
        tmp = start;
        success = MoveToThisDirections(map, positions, tmp, distance.j, directionToMoveCols) && MoveToThisDirections(map, positions, tmp, distance.i, directionToMoveRows); 
    }

    // Si ningun camino funciono devuelvo de una vez la lista vacia
    if(!success) return positions;

    positions.Insert(positions.begin(), start);

    for(auto p : positions){
        std::cout<<"El nodo actual es: "<<p.i<<", "<<p.j<<'\n';
    }
    return positions;
}

Array<Position> Pathfinding::AleatoryMovement(Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny){
  std::cout << "AleatoryMovement desde: " << start.i << "," << start.j 
              << " hasta: " << destiny.i << "," << destiny.j << std::endl;
    
    Array<Position> path = LOS(map, start, destiny);
    std::cout << "LOS path size: " << path.Size() << std::endl;

    if(!path.IsEmpty()){ 
        return path;
    }

    RandomMoves(map, path, start);
    std::cout << "RandomMoves path size: " << path.Size() << std::endl;

    if(!path.IsEmpty()){
        Position newStart = path.Back();
        Array<Position> temp2 = LOS(map, newStart, destiny);
        
        if(!temp2.IsEmpty()){
            path.Insert(path.end(), temp2.begin(), temp2.end());
            return path;
        }
    }
    
    std::cout << "AleatoryMovement fallo, usando BFS como fallback" << std::endl;
    return Bfs(map, start, destiny);
}

Array<Position> Pathfinding::SelectAlgorithm(TypePath path, Element *(&map)[GRID_ROWS][GRID_COLS], const Position &start, const Position &destiny){
    switch (path){
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
        return Array<Position>();
        break;
    }
}