<?php
require_once('solve.php');

$fields = ['a', 'from', 'to', 'algo'];

foreach ($fields as $field)
	if (!isset($_GET[$field]))
	{
		header('Location: index.php');
		exit;
	}
	else
		$$field = $_GET[$field];

$solver = SolverFactory::create($algo);
$solver->calc($solver::prepareMatrixA($a), $from - 1, $to - 1);

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
	</head>
	<body>
		<div class="container">
			<div class="starter-template">
				<h3>Нахождение кратчайшего пути в графе</h3>
				<p class="lead"><?= $solver->name ?></p>

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
			</div>
		</div>

		<script src="js/jquery-3.1.1.min.js"></script>
		<script src="js/tether.min.js"></script>
		<!-- <script src="script.js"></script> -->
		<script src="vendor/twbs/bootstrap/dist/js/bootstrap.min.js"></script>
		<script src="js/ie10-viewport-bug-workaround.js"></script>
	</body>
</html>
