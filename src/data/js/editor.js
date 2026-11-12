$(document).ready(() => {
    M.AutoInit();
    let sd = new showdown.Converter({
        smoothLivePreview: true,
        tables: true,
        strikethrough: true,
        headerLevelStart: 3,
    });

    const $mdInput = $("#md-input");
    const $preview = $("#preview,#preview-split");

    $mdInput.scroll(() => {
        const maxScroll = Math.ceil(
            $mdInput.prop("scrollHeight") - $mdInput.innerHeight()
        );
        const scrollPerc = $mdInput.scrollTop() / maxScroll;
        $preview.scrollTop(
            Math.ceil(
                ($preview.prop("scrollHeight") - $preview.innerHeight()) *
                    scrollPerc
            )
        );
    });

    $mdInput.keyup(() => {
        $preview.html(sd.makeHtml($mdInput.val()));
    });
});
