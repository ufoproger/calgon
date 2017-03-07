var $graphMatrix, $graphSize, $graphFrom, $graphTo, $graphAlgo;

function changeMatrixSize(e)
{		
	if (e === undefined)
		e = {};

	var size = (e.data === undefined) ? $graphSize.val() : e.data.a.length;

	$graphMatrix.empty();

	var $tr = $("<tr>");

	for (var i = 0; i < size; i++)
	{
		$tr = $("<tr>");

		for (var j = 0; j < size; j++)
		{
			var $input = $("<input>", {type: "text", name: "a[" + i + "][" + j + "]", class: "form-control", value: (e.data === undefined) ? "" : (e.data.a[i][j] == 0 ? "" : e.data.a[i][j])});
			$tr.append($("<td>").append($input));
		}

		$graphMatrix.append($tr);
	}

	$graphSize.val(size);
	$graphFrom.empty();
	$graphTo.empty();

	for (var i = 1; i <= size; ++i)
	{
		var $option = $("<option>", {value: i}).text(i);

		$graphFrom.append($option.clone());
		$graphTo.append($option.clone());
	}
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