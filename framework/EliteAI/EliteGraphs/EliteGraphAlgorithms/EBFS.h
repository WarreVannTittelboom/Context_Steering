#pragma once

namespace Elite 
{
	template <class T_NodeType, class T_ConnectionType>
	class BFS
	{
	public:
		BFS(IGraph<T_NodeType, T_ConnectionType>* pGraph);

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);
	private:
		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
	};

	template <class T_NodeType, class T_ConnectionType>
	BFS<T_NodeType, T_ConnectionType>::BFS(IGraph<T_NodeType, T_ConnectionType>* pGraph)
		: m_pGraph(pGraph)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> BFS<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode)
	{
		std::queue<T_NodeType*> openlist; // node still have to connect
		std::map<T_NodeType*, T_NodeType*> closedlist; //hisotyr of made connects

		openlist.push(pStartNode);

		while(!openlist.empty())
		{
			T_NodeType* pCurrentNode = openlist.front();


			//
			openlist.pop();
			if(pCurrentNode == pDestinationNode)
			{
				break;
			}
			for(auto con : m_pGraph->GetNodeConnections(pCurrentNode->GetIndex()))
			{
				T_NodeType* pNextNode = m_pGraph->GetNode(con->GetTo());
				
				if (closedlist.find(pNextNode) == closedlist.end())
				{
					openlist.push(pNextNode);
					closedlist[pNextNode] = pCurrentNode;
				}
			}
		}

		vector<T_NodeType*> path;

		T_NodeType* pCurrentNode = pDestinationNode;
		while(pCurrentNode != pStartNode)
		{
			path.push_back(pCurrentNode);
			pCurrentNode = closedlist[pCurrentNode];

		}
		path.push_back(pStartNode);
		return path;
	}
}

