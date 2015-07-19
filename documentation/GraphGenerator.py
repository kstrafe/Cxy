# -*- coding: utf-8 -*-
# Copyright © 2015 Kevin Robert Stravers
"""
This file is part of Unnamed-Language Compiler Reference Implementation (ULCRI).

ULCRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ULCRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ULCRI.  If not, see <http://www.gnu.org/licenses/>.
"""

import igraph  # To install: `pip3 install python-igraph`

import os
import re


pure_subtrees = {
    'code/codegenerator',
    'code/treebuilder',
    'code/treebuilder/parser',
    'code/treebuilder/parser/Parser.cpp',
    'code/treepruner',

    # Protocols
    'protocols',
    'protocols/Action.hpp',
    'protocols/ConcreteSyntaxTree.hpp',
    'protocols/ConcreteSyntaxTree.cpp',
    'protocols/CrossTerminal.hpp',
    'protocols/EntryType.hpp',
    'protocols/ParseReturn.hpp',
    'protocols/Token.hpp',
    'protocols/TokenType.hpp',
}


uses_global = {

}


pure = '#00FF00'
subtree_pure = '#00BFFF'
impure = 'red'

def createGraph(graph, parent, directory, purity=subtree_pure):
    for root, directories, files in os.walk(directory, topdown=True):
        graph.add_vertex(name=directory)
        if directory in pure_subtrees:
            purity = subtree_pure
            graph.vs[len(graph.vs) - 1]['vertex_color'] = pure
        else:
            graph.vs[len(graph.vs) - 1]['vertex_color'] = purity

        graph.add_edge(source=parent, target=len(graph.vs) - 1)
        root_node = len(graph.vs) - 1
        for file in files:
            if file.startswith('test_'):
                continue
            graph.add_vertex(name=file)
            if os.path.join(directory, file) in pure_subtrees:
                graph.vs[len(graph.vs) - 1]['vertex_color'] = pure
            else:
                graph.vs[len(graph.vs) - 1]['vertex_color'] = purity

            graph.add_edge(source=root_node, target=len(graph.vs) - 1)
        for directory_ in directories:
            createGraph(graph=graph, parent=root_node, directory=os.path.join(root, directory_), purity=purity)
        return


if __name__ == '__main__':
    graph = igraph.Graph()
    graph.to_directed()
    graph.add_vertex(name='Unnamed-Language')

    purity = impure
    directories = ['code', 'libraries', 'protocols']

    for directory in directories:

        for root, directories, files in os.walk(directory, topdown=True):
            graph.add_vertex(name=root)
            graph.add_edge(source=0, target=len(graph.vs) - 1)

            if directory in pure_subtrees:
                purity = subtree_pure
                graph.vs[len(graph.vs) - 1]['vertex_color'] = pure
            else:
                graph.vs[len(graph.vs) - 1]['vertex_color'] = purity

            root_node = len(graph.vs) - 1
            for file in files:
                if file.startswith('test_'):
                    continue
                graph.add_vertex(name=file)
                if os.path.join(directory, file) in pure_subtrees:
                    graph.vs[len(graph.vs) - 1]['vertex_color'] = pure
                    purity = pure
                else:
                    graph.vs[len(graph.vs) - 1]['vertex_color'] = purity

                graph.add_edge(source=root_node, target=len(graph.vs) - 1)
            for directory in directories:
                createGraph(graph=graph, parent=root_node, directory=os.path.join(root, directory), purity=impure)
            break



    layout = graph.layout('kk')
    style = {}

    style['vertex_size'] = 500

    style['vertex_color'] = [vertex['vertex_color'] if vertex['vertex_color'] else 'red' for vertex in graph.vs]
    style['vertex_color'][0] = 'orange'
    style['vertex_label'] = [re.sub('(.{12})', '\\1\n', os.path.basename(name), 0, re.DOTALL) for name in graph.vs['name']]
    style['vertex_label_size'] = 70
    style['edge_width'] = 20
    style['edge_arrow_size'] = 10
    style['edge_curved'] = 0.3
    style['layout'] = layout
    style['bbox'] = (10000, 10000)
    style['margin'] = 300
    igraph.plot(
        graph,
        'documentation/dependency_graph.png', **style)
