import QtQuick
import QtQuick.Controls

Loader {
    id: pageLoader
    property var currentRoute: null
    property bool canLeave: true

    onCurrentRouteChanged: {
        if (currentRoute && currentRoute.page) {
            source = Router.componentForRoute(currentRoute.page);
        } else {
            source = "";
        }
    }

    Connections {
        target: pageLoader.currentRoute

        function onPageChanged() {
            if (pageLoader.currentRoute && pageLoader.currentRoute.page) {
                pageLoader.source = Router.componentForRoute(pageLoader.currentRoute.page);
            }
        }

        function onParamsChanged() {
            if (pageLoader.item && pageLoader.item.hasOwnProperty("routeParams")) {
                pageLoader.item.routeParams = pageLoader.currentRoute.params;
            }
        }
    }

    Component.onCompleted: {
        if (Router.currentRoute.page && Router.componentForRoute(Router.currentRoute.page)) {
            source = Router.componentForRoute(Router.currentRoute.page);
        } else {
            console.log("PageLoader: Router.currentRoute not valid on Component.onCompleted.");
        }
    }

    onLoaded: {
        if (item) {
            if (item.hasOwnProperty("routeParams")) {
                item.routeParams = currentRoute.params;
            }
            if (item.canLeave && typeof item.canLeave === 'function') {
                pageLoader.canLeave = item.canLeave();
            } else {
                pageLoader.canLeave = true;
            }
        }
    }

    onStatusChanged: {
        if (status === Loader.Error) {
            if (errorString !== 'undefined') {
                console.log("PageLoader Error:", errorString);
            } else {
                console.log("PageLoader Unknown Error");
            }
        }
    }
}
