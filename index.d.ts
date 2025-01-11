declare module "deps-qmlui-raub" {
	/**
	 * Exports of `deps-qt-core-raub`
	*/
	export const core: {
		bin: string;
		include: string;
	};
	
	/**
	 * Exports of `deps-qt-gui-raub`
	*/
	export const gui: {
		bin: string;
		include: string;
	};
	
	/**
	 * Exports of `deps-qt-qml-raub`
	*/
	export const qml: {
		bin: string;
		include: string;
	};
	
	/**
	 * Path to binaries
	 *
	 * Platform binary directory absolute path
	*/
	export const bin: string;
	/**
	 * Path to includes
	 *
	 * Include directory for this module
	*/
	export const include: string;
}
