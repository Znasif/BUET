from qgis.core import *
# create a memory layer with two points
layer =  QgsVectorLayer('Point', 'points' , "memory")
# print(help(QgsVectorLayer()))
pr = layer.dataProvider()
# add the first point
pt = QgsFeature()
point1 = QgsPoint(50,50)
pt.setGeometry(QgsGeometry.fromPoint(point1))
pr.addFeatures([pt])
# update extent of the layer
layer.updateExtents()
# add the second point
pt = QgsFeature()
point2 = QgsPoint(100,150)
pt.setGeometry(QgsGeometry.fromPoint(point2))
pr.addFeatures([pt])
# update extent
layer.updateExtents()
# add the layer to the canvas
QgsMapLayerRegistry.instance().addMapLayers([layer])
