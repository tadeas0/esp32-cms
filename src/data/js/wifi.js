$(document).ready(() => {
    listConnections();
    getCurrentConn();
    M.AutoInit();
});

function listConnections() {
    $("#wifi-list").empty();
    $.getJSON("/wifi", (data) => {
        data.forEach((c) => {
            const $btnRow = $("<div>", { class: "row" });
            if (c.known) {
                $btnRow
                    .append(
                        $("<button>", {
                            class: "waves-effect waves-light btn-small col s6",
                            text: "Forget",
                        }).click((e) => {
                            e.preventDefault();
                            handleDeleteConn(c.ssid);
                        })
                    )
                    .append(
                        $("<button>", {
                            class: "waves-effect waves-light btn-small col s6",
                            text: "Connect",
                        }).click((e) => {
                            e.preventDefault();
                            handleConnect(c);
                        })
                    );
            } else {
                $btnRow.append(
                    $("<button>", {
                        class: "waves-effect waves-light btn-small col s12",
                        text: "Connect",
                    }).click((e) => {
                        e.preventDefault();
                        handleConnect(c);
                    })
                );
            }

            $("<li>")
                .append(
                    $("<div>", {
                        class: "collapsible-header",
                        text: c.ssid,
                    })
                )
                .append(
                    $("<div>", {
                        class: "collapsible-body",
                    })
                        .append(
                            $("<ul>", { class: "collection" })
                                .append(
                                    $("<li>", {
                                        class: "collection-item",
                                        text: "SSID",
                                    }).append(
                                        $("<span>", {
                                            class: "right",
                                            text: c.ssid,
                                        })
                                    )
                                )
                                .append(
                                    $("<li>", {
                                        class: "collection-item",
                                        text: "RSSI",
                                    }).append(
                                        $("<span>", {
                                            class: "right",
                                            text: c.rssi,
                                        })
                                    )
                                )
                        )
                        .append($btnRow)
                )
                .appendTo($("#wifi-list"));
        });
    });
}

function handleConnect(connection) {
    let password = connection.password;
    if (!connection.known) {
        password = prompt("Password");
        if (password === null) {
            return;
        }
    }
    const formData = new FormData();
    formData.append("ssid", connection.ssid);
    formData.append("password", password);
    $.ajax({
        url: "/wifi",
        data: formData,
        processData: false,
        contentType: false,
        type: "POST",
    });
    getCurrentConn();
}

function getCurrentConn() {
    $.getJSON("/wifi/current", (data) => {
        $("#wifi-title")
            .text(data.is_connected ? data.ssid : "Not connected")
            .append($("<span>", { class: "badge" }).text(data.status));
        $("#ssid").text(data.ssid);
        $("#ip").text(data.ip);
        $("#rssi").text(data.rssi);
    });
}

function handleDeleteConn(ssid) {
    const formData = new FormData();
    formData.append("ssid", ssid);
    $.ajax({
        url: "/wifi",
        data: formData,
        processData: false,
        contentType: false,
        type: "DELETE",
        success: listConnections,
    });
}
