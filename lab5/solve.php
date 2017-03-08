<?php

abstract class Solver
{
	const INFINITY = 1000000;
	const UNDEFINED_VERTEX = -1;

	public $steps = [];
	public $path = [];
	public $length = 0;
	public $name = "";

	public function pushStep($d, $h, $x = null)
	{
		$step = [];

		array_walk_recursive($d, function(&$item, $key)
		{
				if ($item === self::INFINITY)
					$item = '&infin;';
		});

		array_walk_recursive($h, function(&$item, $key)
		{
			$item++;

			if (!$item)
				$item = '&mdash;';
		});

		$step['D[i]'] = $d;
		$step['H[i]'] = $h;

		if ($x)
		{
			array_walk_recursive($x, function(&$item, $key)
			{
				$item = ($item ? '&#10003;' : '&mdash;');
			});

			$step['X[i]'] = $x;
		}

		$this->steps[] = $step;
	}

	protected function calcPath($h, $from, $to)
	{
		$this->path[] = $to;

		while ($to != $from)
		{
			if ($to == self::UNDEFINED_VERTEX)
			{
				$this->path = [];
				return;
			}

			$to = $h[$to];
			$this->path[] = $to;
		}

		$this->path = array_reverse($this->path);
	}

	static public function prepareMatrixA($a)
	{
		foreach ($a as &$row)
			foreach ($row as &$element)
			{
				if (empty($element))
					$element = self::INFINITY;

				$element = intVal($element);
			}

		return $a;
	}

	static public function prepareMatrixAB($a, $b)
	{
		$a = self::prepareMatrixA($a);
		$b = self::prepareMatrixA($b);
		$size = count($a);

		for ($i = 0; $i < $size; $i++)
			for ($j = 0; $j < $size; $j++)
				$a[$i][$j] = min($a[$i][$j], $b[$i][$j]);
				
		return $a;
	}

	abstract public function calc($a, $from, $to);
}

class SolverDijkstra extends Solver
{
	public $name = "Алгоритм Дейкстры";

	public function calc($a, $from, $to)
	{
		$size = count($a);
		$d = array_fill(0, $size, self::INFINITY);
		$h = array_fill(0, $size, self::UNDEFINED_VERTEX);
		$x = array_fill(0, $size, false);

		$d[$from] = 0;
		$x[$from] = true;
		$previous = $from;

		$this->pushStep($d, $h, $x);

		while (true)
		{
			foreach ($a[$previous] as $j => $weight)
			{
				if ($x[$j])
					continue;

				if ($d[$j] > $d[$previous] + $weight)
				{
					$d[$j] = $d[$previous] + $weight;
					$h[$j] = $previous;
				}
			}

			$min = null;

			foreach ($d as $i => $value)
			{
				if ($x[$i])
					continue;

				if (is_null($min) || $d[$min] > $value)
					$min = $i;
			}

			if (is_null($min))
				break;

			$x[$min] = true;
			$previous = $min;

			$this->pushStep($d, $h, $x);
		}

		$this->calcPath($h, $from, $to);
		$this->length = 0;

		for ($i = 1; $i < count($this->path); $i++)
			$this->length += $a[$this->path[$i - 1]][$this->path[$i]];
	}
}

class SolverFloyd extends Solver
{
	public $name = "Алгоритм Флойда";

	public function calc($a, $from, $to)
	{
		$size = count($a);
		$d = $a;
		$h = [];

		foreach ($a as $i => $row)
		{
			$h[] = [];

			foreach ($row as $j => $element)
				$h[$i][] = ($element == self::INFINITY || $i == $j) ? self::UNDEFINED_VERTEX : $i;
		}

		for ($i = 0; $i < $size; $i++)
			$d[$i][$i] = 0;

		$this->pushStep($d, $h);

		for ($k = 0; $k < $size; $k++)
		{
			$changed = false;

			for ($i = 0; $i < $size; $i++)
				for ($j = 0; $j < $size; $j++)
				{
					if ($i == $k || $j == $k)
						continue;

					if ($d[$i][$k] == self::INFINITY || $d[$k][$j] == self::INFINITY)
						continue;

					if ($d[$i][$j] > $d[$i][$k] + $d[$k][$j])
					{
						$d[$i][$j] = $d[$i][$k] + $d[$k][$j];
						$h[$i][$j] = $h[$k][$j];
						$changed = true;
					}
				}

			if ($changed)
				$this->pushStep($d, $h);

			for ($i = 0; $i < $size; $i++)
				if ($d[$i][$i] < 0)
					return;
		}

		$this->length = $d[$from][$to];
		$this->calcPath($h[$from], $from, $to);
	}
}

class SolverFactory
{
	private function __construct() {}

	public static function create($algo)
	{
		switch ($algo)
		{
			case 'dijkstra':
				return new SolverDijkstra;

			case 'floyd':
				return new SolverFloyd;
		}

		return null;
	}
}
