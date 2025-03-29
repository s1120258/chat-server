function loadDialog(parent, componentName, properties = {}) {
    var component = Qt.createComponent(componentName + ".qml");
    if (component.status != Component.Ready) {
        console.error("Error loading " + componentName + " component");
        return;
    }

    var obj = (properties && Object.keys(properties).length > 0) ?
        component.createObject(parent, properties) :
        component.createObject(parent);

    if (obj === null) {
        console.error("Error creating " + obj + " component");
        return;
    }
    obj.visible = true;
}

function loadWindow(parent, componentName, properties = {}) {
    loadDialog(parent, componentName, properties);
    if (parent) {
        parent.visible = false;
    }
}

function getJsonValue(jsonStr, key) {
    try {
        var jsonObj = JSON.parse(jsonStr);
        return jsonObj[key];
    } catch (e) {
        console.error("Error parsing room JSON string:", e);
        return "";
    }
}
