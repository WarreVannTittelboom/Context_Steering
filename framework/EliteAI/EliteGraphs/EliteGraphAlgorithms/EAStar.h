#pragma once
namespace Elite
{
	template <class T_NodeType, class T_ConnectionType>
	class AStar
	{
	public:
		AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			T_NodeType* pNode = nullptr;
			T_ConnectionType* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);

	private:
		float GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const;

		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
		Heuristic m_HeuristicFunction;
	};

	template <class T_NodeType, class T_ConnectionType>
	AStar<T_NodeType, T_ConnectionType>::AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction)
		: m_pGraph(pGraph)
		, m_HeuristicFunction(hFunction)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> AStar<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pGoalNode)
	{
		vector <T_NodeType*> path;
		vector <NodeRecord> openList;
		vector <NodeRecord> closedList;
		NodeRecord currentRecord{};

		NodeRecord startNode{};
		startNode.pNode = pStartNode;
		startNode.pConnection = nullptr;
		startNode.estimatedTotalCost = GetHeuristicCost(pStartNode, pGoalNode);
		openList.push_back(startNode);
		while (!openList.empty())
		{
			currentRecord = *(std::min_element(openList.begin(), openList.end())); //TODO SET CURRENT TO LOWEST F COST NODE
			
			if (currentRecord.pNode == pGoalNode)
			{
				break; // CHECK IF CONNECTION OF THIS NODE LEADS TO END NODE, IF SO EXIT LOOP
			}
			// ELSE GET ALL CONNECTIONS AND LOOP OVER
			for(auto con : m_pGraph->GetNodeConnections(currentRecord.pNode->GetIndex()))
			{
				//TODO 
				// CALCUALTE TOTAL G COST
				 float CostSoFar = con->GetCost() + currentRecord.costSoFar;
				 	 bool notInClosed{};
			  for (auto Record : closedList)
			  {
				  if( Record.pNode == m_pGraph->GetNode(con->GetTo()))
				  {
					  notInClosed = false;
					  if (Record.costSoFar < CostSoFar) { continue; }
					  else{ closedList.erase(std::remove(closedList.begin(), closedList.end(), Record), closedList.end()); }
				  }
			  }
			  if (notInClosed)
			  {
				  for (auto Record : openList)
				  {
					  if (Record.pNode == m_pGraph->GetNode(con->GetTo()))
					  {
						  if (Record.costSoFar < CostSoFar) { continue; }
						  else { openList.erase(std::remove(openList.begin(), openList.end(), Record), openList.end()); }
					  }
				  }

			  }
				
				NodeRecord NextNode{};
				NextNode.pNode = m_pGraph->GetNode(con->GetTo());
				NextNode.pConnection = con;
				NextNode.costSoFar = CostSoFar; //add costsofar from previous node
				NextNode.estimatedTotalCost = GetHeuristicCost(NextNode.pNode, pGoalNode) + NextNode.costSoFar;
				openList.push_back(NextNode);
				openList.erase(std::remove(openList.begin(), openList.end(), currentRecord), openList.end());
				closedList.push_back(currentRecord);

				

			}
			// EXPENSIVE CON SHOULD BE REMOVED IF IT EXISTED
			// CREATE NEW NODERECORD AND ADDD TO OPENLIST
			// REMOVE OLD NODERECORD FORM OPENLIST AND ADD TO CLOSEDLIST
			

		}
		// RECONSTRUCT PATH

		while (currentRecord.pNode != pStartNode)
		{
			path.push_back(currentRecord.pNode);
			int i{};
			bool found{ false };
			while (!found && i < (int)closedList.size())
			{
				if (closedList[i].pNode == m_pGraph->GetNode(currentRecord.pConnection->GetFrom())) { found = true;  }
				else { ++i; }
			}
			currentRecord = closedList[i];
		}
		path.push_back(pStartNode);
		std::reverse(path.begin(), path.end());
		return path;
	}

	template <class T_NodeType, class T_ConnectionType>
	float Elite::AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const
	{
		Vector2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}
}