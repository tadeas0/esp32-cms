$(document).ready(() => {
    $.getJSON("/system", (data, status) => {
        $("#dns-input").val(data.dns_name);
        $("#ssid-input").val(data.ssid);
        $("#pass-input").val(data.password);
        M.updateTextFields();
    });
});
