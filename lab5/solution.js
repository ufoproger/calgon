var currentStep = 0;

function buildTable(selector, matrix, prevMatrix)
{
	var $table = $(selector);
	$table.empty();

	for (var i = 0; i < matrix.length; i++)
	{
		var $tr = $("<tr>");

		for (var j = 0; j < matrix.length; j++)
			$tr.append($("<td>", {class: (matrix[i][j] == prevMatrix[i][j] ? "" : "table-active")}).html(matrix[i][j]));

		$table.append($tr);
	}
}

function changeStep(e)
{
	e.preventDefault();
	$(this).blur();

	switch (e.data.action)
	{
		case "next":
			++currentStep;
			break;

		case "prev":
			--currentStep;
			break;

		case "start":
			currentStep = 0;
			break;

		case "end":
			currentStep = options.steps.length - 1;
	}

	if (currentStep < 0)
		currentStep = 0;

	if (currentStep >= options.steps.length)
		currentStep = options.steps.length - 1;

	var step = options.steps[currentStep];
	var prevStep = currentStep - 1;

	if (prevStep < 0)
		prevStep = 0;

	prevStep = options.steps[prevStep];

	$("#step-info").html("Шаг " + (currentStep + 1) + " из " + options.steps.length);
	buildTable("#matrix-d", step["D[i]"], prevStep["D[i]"]);
	buildTable("#matrix-h", step["H[i]"], prevStep["H[i]"]);
}

function initAlgoFloyd()
{
	$("#step-prev").on("click", {action: "prev"}, changeStep);
	$("#step-next").on("click", {action: "next"}, changeStep);
	$("#step-start").on("click", {action: "start"}, changeStep).click();
	$("#step-end").on("click", {action: "end"}, changeStep);
}

$(function()
{
	if (options.algo == "floyd")
		initAlgoFloyd();
});
