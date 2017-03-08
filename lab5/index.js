var $graphMatrix, $graphSize, $graphFrom, $graphTo, $graphAlgo;

function buildTable($table, size, name, data)
{
	$table.empty();

	for (var i = 0; i < size; i++)
	{
		var $tr = $("<tr>");

		for (var j = 0; j < size; j++)
		{
			var $input = $("<input>", {
				type: "text",
				name: name + "[" + i + "][" + j + "]",
				class: "form-control form-control-sm",
				value: (data === undefined) ? "" : (data[i][j] == 0 ? "" : data[i][j])
			});

			$tr.append($("<td>").append($input));
		}

		$table.append($tr);
	}
}

function changeMatrixSize(e)
{
	if (e === undefined)
		e = {};

	if (e.data === undefined)
		e.data = {};

	var size = (e.data.a === undefined) ? $graphSize.val() : e.data.a.length;

	buildTable($graphMatrix, size, "a", e.data.a);

	$("#graph-matrix-b").empty();

	if (e.data.b !== undefined)
	{
		$("#matrix-b-title").removeAttr("hidden");
		buildTable($("#graph-matrix-b"), size, "b", e.data.b);
	}
	else
		$("#matrix-b-title").attr("hidden", "hidden");

	$graphSize.val(size);
	$graphFrom.empty();
	$graphTo.empty();

	for (var i = 1; i <= size; ++i)
	{
		var $option = $("<option>", {value: i}).text(i);

		$graphFrom.append($option.clone());
		$graphTo.append($option.clone());
	}

	$graphTo.val(size);
}

$(function()
{
	$graphMatrix = $("#graph-matrix");
	$graphSize = $("#graph-size");
	$graphFrom = $("#graph-from");
	$graphTo = $("#graph-to");
	$graphAlgo = $("#graph-algo");

	$graphSize.on("change", changeMatrixSize);

	$("a.edit-example").on("click", function(e)
	{
		e.preventDefault();
		$(this).blur();

		var index = $(this).data("example-index");
		changeMatrixSize({data: examples[index]});
	});

	changeMatrixSize();
});
