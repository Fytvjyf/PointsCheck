import plotly.graph_objects as go
import plotly.express as px

from sys import argv
import sys, os
import plotly.offline
from PyQt5.QtCore import QUrl
from PyQt5.QtWebEngineWidgets import QWebEngineView
from PyQt5.QtWidgets import QApplication

name_script, path_good, path_wrong = argv

planes_lines = [[]]
good_lines = [[]]
wrong_lines = [[]]

good_color = ['rgb(104,236,170)']
wrong_color = ['rgb(233,109,84)']

def ReadFiles(p_good, p_wrong):
    global good_lines, wrong_lines
    good_lines = ReadFileByLine(p_good)
    wrong_lines = ReadFileByLine(p_wrong)


def ReadFileByLine(path):
    lines = [[]]
    with open(path, "r") as file:
        for line in file:
            values = line.split()
            float_values = []
            for value in values:
                float_values.append(float(value))
            lines.append(float_values)
    return lines

# path_planes = r"C:\Sol\Solution\planes.txt"
# path_good = r"C:\Sol\Solution\points_good.txt"
# path_wrong = r"C:\Sol\Solution\points_wrong.txt"


def CreateFigure(points, _color):
    df = px.data.iris()
    x_list = []
    y_list = []
    z_list = []
    for value in points:
        if (len(value) < 3):
            continue
        x_list.append(value[0])
        y_list.append(value[1])
        z_list.append(value[2])

    fig = px.scatter_3d(df, x=x_list, y=y_list, z=z_list,
            color_discrete_sequence =_color)
    return fig

def show_in_window(fig1, fig2):
    
    plotly.offline.plot(fig1, filename='name1.html', auto_open=False)
    plotly.offline.plot(fig2, filename='name2.html', auto_open=False)
    
    app = QApplication(sys.argv)
    web1 = QWebEngineView()
    web2 = QWebEngineView()
    file1_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "name1.html"))
    file2_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "name2.html"))
    web1.load(QUrl.fromLocalFile(file1_path))
    web2.load(QUrl.fromLocalFile(file2_path))

    web1.show()
    web2.show()
    sys.exit(app.exec_())

ReadFiles(path_good, path_wrong)

show_in_window(
    CreateFigure(good_lines, good_color),
    CreateFigure(wrong_lines, wrong_color))