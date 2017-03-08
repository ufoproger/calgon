<?php
error_reporting(0);

require_once('solver.php');

$fields = ['a', 'from', 'to', 'algo'];

foreach ($fields as $field)
	if (!isset($_GET[$field]))
	{
		header('Location: index.php');
		exit;
	}
	else
		$$field = $_GET[$field];

$superTask = false;
$matrix = [];

if (isset($_GET['b']))
{
	$superTask = true;
	$b = $_GET['b'];
	$matrix = Solver::prepareMatrixAB($a, $b);
}
else
	$matrix = Solver::prepareMatrixA($a);

$solver = SolverFactory::create($algo);

if (!$solver)
	die('Неопознанный алгоритм.');

$solver->calc($matrix, $from - 1, $to - 1);

$options = [
	'algo' => $algo,
	'steps' => $solver->steps,
	'path' => array_map(function($value)
	{
		return $value + 1;
	}, $solver->path),
	'length' => $solver->length,
];

$pathDescribe = [];

if ($superTask && !empty($solver->path))
{
	function getPathType($a, $b, $from, $to, $prevType = 0)
	{
		$l = $a[$from][$to];
		$r = $b[$from][$to];

		if ($l == $r)
			return $prevType;

		return ($l < $r && $l != 0) ? 0 : 1;
	}

	$path = $solver->path;
	$type = ['на поезд', 'в автобус'];
	$pathDescribe = [];
	$prevType = 0;

	for ($i = 1; $i < count($path); $i++)
	{
		$currType = getPathType($a, $b, $path[$i - 1], $path[$i], $prevType);

		if ($currType == $prevType && $i != 1)
			continue;

		$prevType = $currType;
		$pathDescribe[] = sprintf(($i == 1 ? "Посадка %s в городе №%d" : "пересадка %s в городе №%d"), $type[$currType], $path[$i - 1] + 1);
	}

	if (!empty($pathDescribe))
		$pathDescribe[] = 'прибытие в город №' . ($path[count($path) - 1] + 1);
}

$get = $_GET;

if ($get['algo'] == SolverFactory::ALGO_DIJKSTRA)
	$get['algo'] = SolverFactory::ALGO_FLOYD;
else
	$get['algo'] = SolverFactory::ALGO_DIJKSTRA;

?>
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
		<title>Нахождение кратчайшего пути в графе</title>
		<link href="vendor/twbs/bootstrap/dist/css/bootstrap.min.css" rel="stylesheet">
		<link href="style.css" rel="stylesheet">

		<!--[if lt IE 9]>
			<script src="https://cdnjs.cloudflare.com/ajax/libs/html5shiv/3.7.3/html5shiv.js"></script>
		<![endif]-->

		<script type="text/javascript">
			<? printf("var options = %s;", json_encode($options)) ?>
		</script>
	</head>
	<body>
		<div class="container">
			<div class="starter-template">
				<h3><a href="index.php">Нахождение кратчайшего пути в графе</a> из <?= $from ?> в <?= $to ?></h3>
				<p class="lead"><?= $solver->name ?> (<a href="solution.php?<?= http_build_query($get) ?>"><?= SolverFactory::getNameByAlgo($get['algo']) ?></a>)</p>

				<? if (empty($solver->path)): ?>
					<p>Не удалось найти кратчайший путь!</p>
				<? else: ?>
					<p><?= implode(' &rarr; ', $options['path']) ?>, длина <?= $solver->length ?></p>
				<? endif;?>
				<? if (!empty($pathDescribe)): ?>
					<p><?= implode(', ', $pathDescribe) ?></p>
				<? endif; ?>

				<? if ($algo == SolverFactory::ALGO_DIJKSTRA): ?>

					<table class="table table-responsiv">
						<thead class="thead-inverse">
							<tr>
								<th colspan="2">
									Итерация
								</th>
								<th colspan="<?= count($a) ?>">
									Вершины
								</th>
							</tr>
							<tr>
								<th>№</th>
								<th>Метки</th>
								<? for ($i = 1; $i <= count($a); $i++): ?>
									<th><?= $i ?></th>
								<? endfor; ?>
							</tr>
						</thead>
						<tbody>
							<? foreach ($solver->steps as $i => $step): ?>

								<? $isFirstRow = true; ?>

								<? foreach ($step as $label => $row): ?>
									<tr class="<?= ($i % 2 ? "table-active" : "") ?>">
										<? if ($isFirstRow): ?>
											<td rowspan="<?= count($step) ?>"><?= $i ?></td>
											<? $isFirstRow = false; ?>
										<? endif; ?>

										<td><?= $label ?></td>
										<? foreach ($row as $value): ?>
											<td><?= $value ?></td>
										<? endforeach; ?>
									</tr>
								<? endforeach; ?>

							<? endforeach ?>
						</tbody>
					</table>

				<? else: ?>

					<p>
						<span id="step-info">Шаги</span>
					</p>

					<p>
						<a href="#" id="step-prev">&larr; Предыдущий</a>
						<span> | </span>
						<a href="#" id="step-next">Следующий &rarr;</a>
					</p>

					<p>
						<a href="#" id="step-start">В начало</a>
						<span> | </span>
						<a href="#" id="step-end">В конец</a>
					</p>

					<div class="row">
						<div class="col-md-6">
							<p>Матрица D</p>
							<table class="table table-bordered" id="matrix-d"></table>
						</div>
						<div class="col-md-6">
							<p>Матрица H</p>
							<table class="table table-bordered" id="matrix-h"></table>
						</div>
					</div>
				<? endif; ?>
			</div>
		</div>

		<script src="js/jquery-3.1.1.min.js"></script>
		<script src="js/tether.min.js"></script>
		<script src="solution.js"></script>
		<script src="vendor/twbs/bootstrap/dist/js/bootstrap.min.js"></script>
		<script src="js/ie10-viewport-bug-workaround.js"></script>
	</body>
</html>
