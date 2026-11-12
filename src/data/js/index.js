$(document).ready(setArticleList);

async function setArticleList() {
    const $artList = $("#article-list");
    $.getJSON("/article", (data, status) => {
        $artList.empty();
        $.each(data, (i, article) => {
            $artList.empty();
            $.each(data, (i, article) => {
                const name = decodeURIComponent(article.name);

                $("<div>", { class: "card" })
                    .append(
                        $("<div>", { class: "card-content" }).append(
                            $("<span>", { class: "card-title" }).append(
                                $("<a>", {
                                    href: article.link,
                                    text: name,
                                })
                            )
                        )
                    )
                    .appendTo($artList);
            });
        });
    });
}
