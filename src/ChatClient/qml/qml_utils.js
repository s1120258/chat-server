function loadDialog(parent, componentName) {
    var component = Qt.createComponent(componentName + ".qml");
    if (component.status != Component.Ready) {
        console.error("Error loading " + componentName + " component");
        return;
    }

    var obj = component.createObject(parent);
    if (obj === null) {
        console.error("Error creating " + obj + " component");
        return;
    }
    obj.visible = true;
}
function loadWindow(parent, componentName) {
    loadDialog(parent, componentName);
    if (parent) {
        parent.visible = false;
    }
}