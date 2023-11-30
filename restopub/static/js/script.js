const toggleMenu = () => {
    $(document).ready(function () {
        $(".nav__bar_button").click(function () {
            var d = $(this).attr("id")
            console.log(d)
        })
    })
};

const reserve = () => {
    $(document).ready(function () {
        $(".nav__bar_button").click(function () {
            var d = $(this).attr("id")
            $.ajax({
                type: "GET",
                url: "/load_menu/" + reservation + "/" + quantity,
                contentType: "application/json",
                data: { reservation: reservation, quantity: quantity },
                success: function (response) {
                    console.log("success");
                },
                error: function (response) {
                    console.log("error");
                }
            })
        })
    })
};

const copyToClipBoard = () => {
    $(document).ready(function () {
        $(".phone").click(function () {
            document.execCommand("copy");
        })
    })
};


toggleMenu();
reserve();