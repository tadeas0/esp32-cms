$(document).ready(() => {
    M.AutoInit();
    setArticleList();
    setMemCard();
});

function deleteArticle(encodedName) {
    $.ajax({
        url: `/article/${encodedName}`,
        type: "DELETE",
        success: setArticleList,
    });
}

function setMemCard() {
    $.getJSON("/system", (data, status) => {
        const width = Math.ceil((data.used_bytes / data.total_bytes) * 100);
        $("#mem-bar").css("width", width + "%");
        const lo = (data.used_bytes / 1000000).toFixed(2);
        const hi = (data.total_bytes / 1000000).toFixed(2);
        $("#mem-text").text(lo + " / " + hi + " MiB");
    });
}

function setArticleList() {
    const $artList = $("#article-list");
    $.getJSON("/article", (data, status) => {
        $artList.empty();
        $.each(data, (i, article) => {
            const name = decodeURIComponent(article.name);

            $("<div>", { class: "card" })
                .append(
                    $("<div>", { class: "card-content" }).append(
                        $("<span>", { class: "card-title" })
                            .append(
                                $("<a>", {
                                    href: article.link,
                                    text: name,
                                })
                            )
                            .append(
                                $("<button>", {
                                    class: "btn-flat right",
                                })
                                    .click((e) => {
                                        e.preventDefault();
                                        deleteArticle(article.name);
                                    })
                                    .append(
                                        $("<i>", {
                                            class: "material-icons",
                                            text: "delete",
                                        })
                                    )
                            )
                            .append(
                                $("<a>", {
                                    class: "btn-flat right",
                                    href: article.editLink,
                                }).append(
                                    $("<i>", {
                                        class: "material-icons",
                                        text: "edit",
                                    })
                                )
                            )
                    )
                )
                .appendTo($artList);
        });
    });
}
