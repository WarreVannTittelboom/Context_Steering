#pragma once
#include <stack>

namespace Elite
{
	enum class Eulerianity
	{
		notEulerian, // NO path
		semiEulerian, // Trail, but no cycle
		eulerian, // Cycle
	};

	template <class T_NodeType, class T_ConnectionType>
	class EulerianPath
	{
	public:

		EulerianPath(IGraph<T_NodeType, T_ConnectionType>* pGraph);

		Eulerianity IsEulerian() const;
		vector<T_NodeType*> FindPath(Eulerianity& eulerianity) const;

	private:
		void VisitAllNodesDFS(int startIdx, vector<bool>& visited) const;
		bool IsConnected() const;

		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
	};

	template<class T_NodeType, class T_ConnectionType>
	inline EulerianPath<T_NodeType, T_ConnectionType>::EulerianPath(IGraph<T_NodeType, T_ConnectionType>* pGraph)
		: m_pGraph(pGraph)
	{
	}

	template<class T_NodeType, class T_ConnectionType>
	inline Eulerianity EulerianPath<T_NodeType, T_ConnectionType>::IsEulerian() const
	{
		// If the graph is not connected, there can be no Eulerian Trail
		if (!IsConnected())
		{
			return Eulerianity::notEulerian;
		}

		// Count nodes with odd degree 
		auto activeNodes = m_pGraph->GetAllActiveNodes();
		int oddCount = 0;

		for (auto activeNode : activeNodes)
		{
			auto connections = m_pGraph->GetNodeConnections(activeNode);

			// If last bit 1 -> uneven
			if (connections.size() & 1) oddCount++;
		}


		// A connected graph with more than 2 nodes with an odd degree (an odd amount of connections) is not Eulerian
		if (oddCount > 2) return Eulerianity::notEulerian;

		// A connected graph with exactly 2 nodes with an odd degree is Semi-Eulerian (unless there are only 2 nodes)
		// An Euler trail can be made, but only starting and ending in these 2 nodes
		if (oddCount == 2) return Eulerianity::semiEulerian;

		// You cannot have a connected graph with 1 odd node

		// A connected graph with no odd nodes is Eulerian
		return Eulerianity::eulerian;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline vector<T_NodeType*> EulerianPath<T_NodeType, T_ConnectionType>::FindPath(Eulerianity& eulerianity) const
	{
		// Get a copy of the graph because this algorithm involves removing edges
		auto graphCopy = m_pGraph->Clone();
		auto path = vector<T_NodeType*>();
		int nrOfNodes = graphCopy->GetNrOfNodes();

		// Check if there can be an Euler path
		// If this graph is not eulerian, return the empty path
		// Else we need to find a valid starting index for the algorithm
		auto isEulerian = IsEulerian();
		int startingNodeIdx = invalid_node_index;

		//switch (isEulerian)
		//{
		//case Elite::Eulerianity::notEulerian:
		//	return path;
		//case Elite::Eulerianity::semiEulerian:
		//	// Find First Odd One
		//	auto activeNodes = m_pGraph->GetAllActiveNodes();

		//	for (auto activeNode : activeNodes)
		//	{
		//		auto connections = graphCopy->GetNodeConnections(activeNode);

		//		// If last bit 1 -> uneven
		//		if (connections.size() & 1)
		//		{
		//			startingNodeIdx = activeNode->GetIndex();
		//			break;
		//		}
		//	}
		//	break;
		//case Elite::Eulerianity::eulerian:
		//	startingNodeIdx = 0;
		//	break;
		//default:
		//	break;
		//}

		// Start algorithm loop
		stack<int> nodeStack;


		return path;
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void EulerianPath<T_NodeType, T_ConnectionType>::VisitAllNodesDFS(int startIdx, vector<bool>& visited) const
	{
		// mark the visited node
		visited[startIdx] = true;

		// recursively visit any valid connected nodes that were not visited before
		for (T_ConnectionType* connection : m_pGraph->GetNodeConnections(startIdx))
		{
			if (!visited[connection->GetTo()])
				VisitAllNodesDFS(connection->GetTo(), visited);
		}

	}

	template<class T_NodeType, class T_ConnectionType>
	inline bool EulerianPath<T_NodeType, T_ConnectionType>::IsConnected() const
	{
		auto activeNodes = m_pGraph->GetAllActiveNodes();
		vector<bool> visited(m_pGraph->GetNrOfNodes(), false);

		// find a valid starting node that has connections
		int connectedIdx = invalid_node_index;
		for (auto node : activeNodes)
		{
			auto connections = m_pGraph->GetNodeConnections(node);
			if (connections.size() > 0)
			{
				connectedIdx = node->GetIndex();
				break;
			}
		}

		// if no valid node could be found, return false
		if (connectedIdx == invalid_node_index) return false;

		// start a depth-first-search traversal from the node that has at least one connection
		VisitAllNodesDFS(connectedIdx, visited);

		// if a node was never visited, this graph is not connected
		for (auto node : activeNodes)
		{
			if (!visited[node->GetIndex()]) return false;
		}

		return true;
	}

}